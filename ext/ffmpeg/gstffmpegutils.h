/* GStreamer
 * Copyright (C) <2009> Edward Hervey <bilboed@bilboed.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GST_FFMPEG_UTILS_H__
#define __GST_FFMPEG_UTILS_H__

#ifdef HAVE_FFMPEG_UNINSTALLED
#include <avcodec.h>
#else
#include <libavcodec/avcodec.h>
#endif
#include <gst/gst.h>


#define EDGE_WIDTH avcodec_get_edge_width()


/*
 *Get the size of an picture
 */
int
gst_ffmpeg_avpicture_get_size (int pix_fmt, int width, int height,
        gboolean use_border);

/*
 * Fill in pointers in an AVPicture, aligned by 4 (required by X).
 */

int
gst_ffmpeg_avpicture_fill (AVPicture * picture,
                           uint8_t *   ptr,
                           enum PixelFormat pix_fmt,
                           int         width,
                           int         height,
                           gboolean    use_border);

/*
 * Convert from/to a GStreamer <-> FFMpeg timestamp.
 */
static inline guint64
gst_ffmpeg_time_ff_to_gst (gint64 pts, AVRational base)
{
  guint64 out;

  if (pts == AV_NOPTS_VALUE){
    out = GST_CLOCK_TIME_NONE;
  } else {
    AVRational bq = { 1, GST_SECOND };
    out = av_rescale_q (pts, base, bq);
  }

  return out;
}

static inline gint64
gst_ffmpeg_time_gst_to_ff (guint64 time, AVRational base)
{
  gint64 out;

  if (!GST_CLOCK_TIME_IS_VALID (time) || base.num == 0) {
    out = AV_NOPTS_VALUE;
  } else {
    AVRational bq = { 1, GST_SECOND };
    out = av_rescale_q (time, bq, base);
  }

  return out;
}

void 
gst_ffmpeg_init_pix_fmt_info(void);


G_CONST_RETURN gchar *
gst_ffmpeg_get_codecid_longname (enum CodecID codec_id);

gint
av_smp_format_depth(enum SampleFormat smp_fmt);

GstBuffer *
new_aligned_buffer (gint size, GstCaps * caps);

#endif /* __GST_FFMPEG_UTILS_H__ */
