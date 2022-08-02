/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2022, Ideas on Board Oy.
 *
 * stream_colorspace.cpp - Stream colorspace tests
 */

#include <iostream>

#include <libcamera/formats.h>
#include <libcamera/stream.h>

#include "test.h"

using namespace libcamera;

class StreamColorSpaceTest : public Test
{
protected:
	int run()
	{
		StreamConfiguration cfg;
		cfg.size = { 640, 320 };
		cfg.pixelFormat = formats::YUV422;
		cfg.colorSpace = ColorSpace::Srgb;

		/* YUV stream with sRGB colorspace should have y'cbcr encoding adjusted */
		ColorSpace adjColorSpace = cfg.colorSpace->adjust(cfg);
		if (adjColorSpace.ycbcrEncoding == ColorSpace::YcbcrEncoding::None)
			return TestFail;

		/* For RGB pixelFormat, sRGB colorspace shouldn't get adjusted */
		cfg.pixelFormat = formats::RGB888;
		adjColorSpace = cfg.colorSpace->adjust(cfg);
		if (adjColorSpace == ColorSpace::Srgb)
			;
		else
			return TestFail;

		/*
		 * For YUV pixelFormat, encoding should picked up according to
		 * primaries.
		 */
		cfg.pixelFormat = formats::YUV422;
		cfg.colorSpace = ColorSpace(ColorSpace::Primaries::Rec2020,
					    ColorSpace::TransferFunction::Rec709,
					    ColorSpace::YcbcrEncoding::None,
					    ColorSpace::Range::Limited);
		adjColorSpace = cfg.colorSpace->adjust(cfg);
		if (adjColorSpace.ycbcrEncoding != ColorSpace::YcbcrEncoding::Rec2020)
			return TestFail;

		return TestPass;
	}
};

TEST_REGISTER(StreamColorSpaceTest)
