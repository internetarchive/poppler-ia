/* poppler-document.h: glib interface to poppler
 * Copyright (C) 2004, Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __POPPLER_DOCUMENT_H__
#define __POPPLER_DOCUMENT_H__

#include <glib-object.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#include "poppler.h"

G_BEGIN_DECLS

#define POPPLER_TYPE_DOCUMENT             (poppler_document_get_type ())
#define POPPLER_DOCUMENT(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), POPPLER_TYPE_DOCUMENT, PopplerDocument))
#define POPPLER_IS_DOCUMENT(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), POPPLER_TYPE_DOCUMENT))

typedef enum
{
  POPPLER_PAGE_LAYOUT_UNSET,
  POPPLER_PAGE_LAYOUT_SINGLE_PAGE,
  POPPLER_PAGE_LAYOUT_ONE_COLUMN,
  POPPLER_PAGE_LAYOUT_TWO_COLUMN_LEFT,
  POPPLER_PAGE_LAYOUT_TWO_COLUMN_RIGHT,
  POPPLER_PAGE_LAYOUT_TWO_PAGE_LEFT,
  POPPLER_PAGE_LAYOUT_TWO_PAGE_RIGHT,
} PopplerPageLayout;

typedef enum
{
  POPPLER_PAGE_MODE_UNSET,
  POPPLER_PAGE_MODE_NONE,
  POPPLER_PAGE_MODE_USE_OUTLINES,
  POPPLER_PAGE_MODE_USE_THUMBS,
  POPPLER_PAGE_MODE_FULL_SCREEN,
  POPPLER_PAGE_MODE_USE_OC,
  POPPLER_PAGE_MODE_USE_ATTACHMENTS,
} PopplerPageMode;

typedef enum /*< flags >*/
{
  POPPLER_VIEWER_PREFERENCES_UNSET = 0,
  POPPLER_VIEWER_PREFERENCES_HIDE_TOOLBAR = 1 << 0,
  POPPLER_VIEWER_PREFERENCES_HIDE_MENUBAR = 1 << 1,
  POPPLER_VIEWER_PREFERENCES_HIDE_WINDOWUI = 1 << 2,
  POPPLER_VIEWER_PREFERENCES_FIT_WINDOW = 1 << 3,
  POPPLER_VIEWER_PREFERENCES_CENTER_WINDOW = 1 << 4,
  POPPLER_VIEWER_PREFERENCES_DISPLAY_DOC_TITLE = 1 << 5,
  POPPLER_VIEWER_PREFERENCES_DIRECTION_RTL = 1 << 6,
} PopplerViewerPreferences;



PopplerDocument *poppler_document_new_from_file     (const char       *uri,
						     const char       *password,
						     GError          **error);
gboolean         poppler_document_save              (PopplerDocument  *document,
						     const char       *uri,
						     GError          **error);
int              poppler_document_get_n_pages       (PopplerDocument  *document);
PopplerPage     *poppler_document_get_page          (PopplerDocument  *document,
						     int               page);
PopplerPage     *poppler_document_get_page_by_label (PopplerDocument  *document,
						     const char       *label);

GType            poppler_document_get_type           (void) G_GNUC_CONST;

/* Interface for getting the Index of a poppler_document */
PopplerIndexIter *poppler_index_iter_new        (PopplerDocument   *document);
PopplerIndexIter *poppler_index_iter_get_child  (PopplerIndexIter  *parent);
PopplerAction    *poppler_index_iter_get_action (PopplerIndexIter  *iter);
gboolean          poppler_index_iter_next       (PopplerIndexIter  *iter);
void              poppler_index_iter_free       (PopplerIndexIter  *iter);

/* Export to ps */
PopplerPSFile *poppler_ps_file_new   (PopplerDocument *document,
				      const char      *filename,
				      int              n_pages);
void           poppler_ps_file_free  (PopplerPSFile   *ps_file);

G_END_DECLS

#endif /* __POPPLER_DOCUMENT_H__ */