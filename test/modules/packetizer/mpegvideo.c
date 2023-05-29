/*****************************************************************************
 * mpegvideo.c: H262 packetizer unit testing
 *****************************************************************************
 * Copyright (C) 2019 VideoLabs, VideoLAN and VLC Authors
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc/vlc.h>
#include "../../../lib/libvlc_internal.h"
#include "../../libvlc/test.h"

#include "packetizer.h"

/* 16x16, keyint 25, 50 frames */

const uint8_t test_samples_raw_mpeg[] = {
  0x00, 0x00, 0x01, 0xba, 0x21, 0x00, 0x01, 0x00, 0x01, 0xc3, 0x33, 0x67,
  0x00, 0x00, 0x01, 0xbb, 0x00, 0x09, 0xc3, 0x33, 0x67, 0x00, 0x21, 0xff,
  0xe0, 0xe0, 0xe6, 0x00, 0x00, 0x01, 0xe0, 0x04, 0xfc, 0x31, 0x00, 0x03,
  0x7b, 0xb1, 0x11, 0x00, 0x03, 0x5f, 0x91, 0x00, 0x00, 0x01, 0xb3, 0x01,
  0x00, 0x10, 0x13, 0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x01, 0xb5, 0x14,
  0x8a, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0xb8, 0x00, 0x08, 0x00,
  0x40, 0x00, 0x00, 0x01, 0x00, 0x00, 0x0f, 0xff, 0xf8, 0x00, 0x00, 0x01,
  0xb5, 0x8f, 0xff, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x13, 0xf8,
  0x7d, 0x29, 0x48, 0x88, 0x00, 0x00, 0x01, 0x00, 0x00, 0x57, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x00, 0x97, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x00, 0xd7, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x01, 0x17, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x01, 0x57, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x01, 0x97, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x01, 0xd7, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x02, 0x17, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x02, 0x57, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x02, 0x97, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x02, 0xd7, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x03, 0x17, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x03, 0x57, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x03, 0x97, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x03, 0xd7, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x04, 0x17, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x04, 0x57, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x04, 0x97, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x04, 0xd7, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x05, 0x17, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x05, 0x57, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x05, 0x97, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x05, 0xd7, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0x00, 0x06, 0x17, 0xff, 0xfb,
  0x80, 0x00, 0x00, 0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00,
  0x01, 0x01, 0x12, 0x70, 0x00, 0x00, 0x01, 0xb3, 0x01, 0x00, 0x10, 0x13,
  0xff, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x01, 0xb5, 0x14, 0x8a, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x01, 0xb8, 0x00, 0x08, 0x20, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x0f, 0xff, 0xf8, 0x00, 0x00, 0x01, 0xb5, 0x8f, 0xff,
  0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x13, 0xf8, 0x7d, 0x29, 0x48,
  0x88, 0x00, 0x00, 0x01, 0x00, 0x00, 0x57, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x00, 0x97, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x00, 0xd7, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x01, 0x17, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x01, 0x57, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x01, 0x97, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x01, 0xd7, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x02, 0x17, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x02, 0x57, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x02, 0x97, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x02, 0xd7, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x03, 0x17, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x03, 0x57, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x03, 0x97, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x03, 0xd7, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x04, 0x17, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x04, 0x57, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x04, 0x97, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x04, 0xd7, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x05, 0x17, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x05, 0x57, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x05, 0x97, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x05, 0xd7, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70, 0x00, 0x00, 0x01, 0x00, 0x06, 0x17, 0xff, 0xfb, 0x80, 0x00, 0x00,
  0x01, 0xb5, 0x81, 0x1f, 0xf3, 0x41, 0x80, 0x00, 0x00, 0x01, 0x01, 0x12,
  0x70
};
const size_t test_samples_raw_mpeg_len = 1309;

int main(void)
{
    test_init();

    libvlc_instance_t *vlc = libvlc_new(0, NULL);
    if(!vlc)
        return 1;

    struct params_s params;
    params.vlc = vlc;
    params.obj = VLC_OBJECT(vlc->p_libvlc_int);
    params.codec = VLC_CODEC_MPGV;
    params.i_rate_num = 0;
    params.i_rate_den = 0;
    params.i_frame_count = 2*25;
    params.b_extra = false;

    params.i_read_size = 500;
    RUN("block 500", test_packetize,
        test_samples_raw_mpeg, test_samples_raw_mpeg_len, 0);

    params.i_rate_num = 60000;
    params.i_rate_den = 1001;
    params.i_read_size = 8;
    RUN("block 8", test_packetize,
        test_samples_raw_mpeg, test_samples_raw_mpeg_len, 0);

    params.i_frame_count = 1*25;
    params.i_read_size = 500;
    RUN("skip 1st Iframe", test_packetize,
        test_samples_raw_mpeg + 100, test_samples_raw_mpeg_len - 100, 0);

    libvlc_release(vlc);
    return 0;
}
