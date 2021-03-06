/*
 * jabberd - Jabber Open Source Server
 * Copyright (c) 2002 Jeremie Miller, Thomas Muldowney,
 *                    Ryan Eatmon, Robert Norris
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA02111-1307USA
 */

#include "sx.h"
#include <lib/util.h>

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <gc.h>

sx_t *sx_new(sx_env_t *env, const char *ip, int port, sx_callback_t cb, void *arg) {
    sx_t *s;
    size_t i;

    assert((int) (cb != NULL));

    s = (sx_t*) GC_MALLOC(sizeof(struct sx_st));

    s->env = env;
    s->ip = ip;
    s->port = port;
    s->cb = cb;
    s->cb_arg = arg;

    s->expat = XML_ParserCreateNS(NULL, '|');
    XML_SetReturnNSTriplet(s->expat, 1);
    XML_SetUserData(s->expat, (void *) s);
    /* Prevent the "billion laughs" attack against expat by disabling
     * internal entity expansion.  With 2.x, forcibly stop the parser
     * if an entity is declared - this is safer and a more obvious
     * failure mode.  With older versions, simply prevent expenansion
     * of such entities. */
#ifdef HAVE_XML_STOPPARSER
    XML_SetEntityDeclHandler(s->expat, (void *) _sx_entity_declaration);
#else
    XML_SetDefaultHandler(s->expat, NULL);
#endif

#ifdef HAVE_XML_SETHASHSALT
    XML_SetHashSalt(s->expat, rand());
#endif

    s->wbufq = jqueue_new();
    s->rnadq = jqueue_new();

    if (env != NULL) {
        s->plugin_data = (void **) GC_MALLOC(sizeof(void *) * env->nplugins);

        for (i = 0; i < env->nplugins; i++)
            if (env->plugins[i]->new != NULL)
                (env->plugins[i]->new)(s, env->plugins[i]);
    }

    _sx_debug("allocated new sx for %s:%d", ip, port);

    return s;
}

void sx_free(sx_t *s) {
    sx_buf_t *buf;
    nad_t *nad;
    size_t i;
    _sx_chain_t *scan, *next;

    if (s == NULL)
        return;

    /* we are not reentrant */
    assert(!s->reentry);

    _sx_debug("freeing sx for %s:%d", s->ip, s->port);

    if (s->ns != NULL) GC_FREE((void*)s->ns);

    if (s->req_to != NULL) GC_FREE((void*)s->req_to);
    if (s->req_from != NULL) GC_FREE((void*)s->req_from);
    if (s->req_version != NULL) GC_FREE((void*)s->req_version);

    if (s->res_to != NULL) GC_FREE((void*)s->res_to);
    if (s->res_from != NULL) GC_FREE((void*)s->res_from);
    if (s->res_version != NULL) GC_FREE((void*)s->res_version);

    if (s->id != NULL) GC_FREE((void*)s->id);

    while((buf = jqueue_pull(s->wbufq)) != NULL)
        _sx_buffer_free(buf);
    if (s->wbufpending != NULL)
        _sx_buffer_free(s->wbufpending);

    while((nad = jqueue_pull(s->rnadq)) != NULL)
        nad_free(nad);

    jqueue_free(s->wbufq);
    jqueue_free(s->rnadq);

    XML_ParserFree(s->expat);

    if (s->nad != NULL) nad_free(s->nad);

    if (s->auth_method != NULL) GC_FREE((void*)s->auth_method);
    if (s->auth_id != NULL) GC_FREE((void*)s->auth_id);

    if (s->env != NULL) {
        _sx_debug("freeing %zu env plugins", s->env->nplugins);
        for (i = 0; i < s->env->nplugins; i++)
            if (s->env->plugins[i]->free != NULL)
                (s->env->plugins[i]->free)(s, s->env->plugins[i]);

        scan = s->wio;
        while(scan != NULL) {
            next = scan->wnext;
            GC_FREE(scan);
            scan = next;
        }

        scan = s->wnad;
        while(scan != NULL) {
            next = scan->wnext;
            GC_FREE(scan);
            scan = next;
        }

        GC_FREE(s->plugin_data);
    }

    GC_FREE(s);
}

/** force advance into auth state */
void sx_auth(sx_t *s, const char *auth_method, const char *auth_id) {
    assert((int) (s != NULL));

    _sx_debug("authenticating stream (method=%s; id=%s)", auth_method, auth_id);

    if (auth_method != NULL) s->auth_method = GC_STRDUP(auth_method);
    if (auth_id != NULL) s->auth_id = GC_STRDUP(auth_id);

    _sx_state(s, state_OPEN);
    _sx_event(s, event_OPEN, NULL);
}

/** utility; reset stream state */
void _sx_reset(sx_t *s) {
    struct sx_st temp;
    sx_t *new;

    _sx_debug("resetting stream state");

    /* we want to reset the contents of s, but we can't free s because
     * the caller (and others) hold references. so, we make a new sx_t,
     * copy the contents (only pointers), free it (which will free strings
     * and queues), then make another new one, and copy the contents back
     * into s */

    temp.env = s->env;
    temp.ip = s->ip;
    temp.port = s->port;
    temp.cb = s->cb;
    temp.cb_arg = s->cb_arg;

    temp.flags = s->flags;
    temp.reentry = s->reentry;
    temp.ssf = s->ssf;
    temp.wio = s->wio;
    temp.rio = s->rio;
    temp.wnad = s->wnad;
    temp.rnad = s->rnad;
    temp.rbytesmax = s->rbytesmax;
    temp.plugin_data = s->plugin_data;

    s->reentry = 0;

    s->env = NULL;  /* we get rid of this, because we don't want plugin data to be freed */

    new = (sx_t*) GC_MALLOC(sizeof(struct sx_st));
    memcpy(new, s, sizeof(struct sx_st));
    sx_free(new);

    new = sx_new(NULL, temp.ip, temp.port, temp.cb, temp.cb_arg);
    memcpy(s, new, sizeof(struct sx_st));
    GC_FREE(new);

    /* massaged expat into shape */
    XML_SetUserData(s->expat, (void *) s);

    s->env = temp.env;
    s->flags = temp.flags;
    s->reentry = temp.reentry;
    s->ssf = temp.ssf;
    s->wio = temp.wio;
    s->rio = temp.rio;
    s->wnad = temp.wnad;
    s->rnad = temp.rnad;
    s->rbytesmax = temp.rbytesmax;
    s->plugin_data = temp.plugin_data;

    s->has_reset = 1;

    _sx_debug("finished resetting stream state");
}

/** utility: make a new buffer
   if len>0 but data is NULL, the buffer will contain that many bytes
   of garbage, to be overwritten by caller. otherwise, data pointed to
   by 'data' will be copied into buf */
sx_buf_t *_sx_buffer_new(const char *data, int len, _sx_notify_t notify, void *notify_arg) {
    sx_buf_t *buf;

    buf = (sx_buf_t*) GC_MALLOC(sizeof(struct sx_buf_st));

    if (len <= 0) {
        buf->data = buf->heap = NULL;
        buf->len = 0;
    } else {
        buf->data = buf->heap = (char *) GC_MALLOC_ATOMIC_IGNORE_OFF_PAGE(sizeof(char) * len);
        if (data != NULL)
            memcpy(buf->data, data, len);
        else
            memset(buf->data, '$', len);  /* catch uninitialized use */
        buf->len = len;
    }

    buf->notify = notify;
    buf->notify_arg = notify_arg;

    return buf;
}

/** utility: kill a buffer */
void _sx_buffer_free(sx_buf_t *buf) {
    if (buf->heap != NULL)
        GC_FREE(buf->heap);

    GC_FREE(buf);
}

/** utility: clear out a buffer, but don't deallocate it */
void _sx_buffer_clear(sx_buf_t *buf) {
    if (buf->heap != NULL) {
        GC_FREE(buf->heap);
        buf->heap = NULL;
    }
    buf->data = NULL;
    buf->len = 0;
}

/** utility: ensure a certain amount of allocated space adjacent to buf->data */
void _sx_buffer_alloc_margin(sx_buf_t *buf, size_t before, size_t after)
{
    char *new_heap;

    /* If there wasn't any data in the buf, we can just allocate space for the margins */
    if (buf->data == NULL || buf->len == 0) {
        if (buf->heap != NULL)
            buf->heap = GC_REALLOC(buf->heap, before+after);
        else
            buf->heap = GC_MALLOC_ATOMIC_IGNORE_OFF_PAGE(before+after);
        buf->data = buf->heap + before;
        return;
    }

    if (buf->heap != NULL) {
        assert(buf->data >= buf->heap);
        unsigned int old_leader = buf->data - buf->heap;
        /* Hmmm, maybe we can just call realloc() ? */
        if (old_leader >= before && old_leader <= (before * 4)) {
            buf->heap = GC_REALLOC(buf->heap, before + buf->len + after);
            buf->data = buf->heap + old_leader;
            return;
        }
    }

    /* Most general case --- allocate a new buffer, copy stuff over, free the old one. */
    new_heap = GC_MALLOC_ATOMIC(before + buf->len + after);
    memcpy(new_heap + before, buf->data, buf->len);
    if (buf->heap != NULL)
        GC_FREE(buf->heap);
    buf->heap = new_heap;
    buf->data = new_heap + before;
}

/** utility: reset a sx_buf_t's contents. If newheap is non-NULL it is assumed to be 'data's malloc block and ownership of the block is taken by the buffer. If newheap is NULL then the data is copied. */
void _sx_buffer_set(sx_buf_t *buf, char* newdata, int newlength, char* newheap)
{
    if (newheap == NULL) {
        buf->len = 0;
        _sx_buffer_alloc_margin(buf, 0, newlength);
        if (newlength > 0)
            memcpy(buf->data, newdata, newlength);
        buf->len = newlength;
        return;
    }

    _sx_buffer_clear(buf);
    buf->data = newdata;
    buf->len = newlength;
    buf->heap = newheap;
}

int __sx_event(sx_t *s, sx_event_t e, void *data) {
    int ret;

    _sx_debug("%s:%d event %d data %p", s->ip, s->port, e, data);

    s->reentry++;
    ret = (s->cb)(s, e, data, s->cb_arg);
    s->reentry--;

    return ret;
}

/** show sx flags as string - for logging */
char *_sx_flags(sx_t *s) {
    static char flags[256];
    flags[1] = '\0';
    snprintf(flags, sizeof(flags), "%s%s%s",
             s->ssf ? ",TLS" : "",
             (s->flags & SX_COMPRESS_WRAPPER) ? ",ZLIB" : "",
             (s->flags & SX_WEBSOCKET_WRAPPER) ? ",WS" : ""
            );
    return flags + 1;
}
