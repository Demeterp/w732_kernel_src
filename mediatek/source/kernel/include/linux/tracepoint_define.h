/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#ifndef _LINUX_TRACEPOINT_H
#include <linux/tracepoint.h>
#else

#ifndef _TRACEPOINT_DEFINE
#define _TRACEPOINT_DEFINE

#ifdef CONFIG_TRACEPOINTS
/*
 * Make sure the alignment of the structure in the __tracepoints section will
 * not add unwanted padding between the beginning of the section and the
 * structure. Force alignment to the same alignment as the section start.
 */
#undef TP_PROTO
#define TP_PROTO(args...)	args
#undef TP_ARGS
#define TP_ARGS(args...)	args
#undef PARAMS
#define PARAMS(args...) args

#undef DECLARE_TRACE
#define DECLARE_TRACE(name, proto, args)				\
	extern struct tracepoint __tracepoint_##name;			\
	static inline void trace_##name(proto)				\
	{								\
		if (unlikely(__tracepoint_##name.state))		\
			__DO_TRACE(&__tracepoint_##name,		\
				TP_PROTO(proto), TP_ARGS(args));	\
	}								\
	static inline int register_trace_##name(void (*probe)(proto))	\
	{								\
		return tracepoint_probe_register(#name, (void *)probe);	\
	}								\
	static inline int unregister_trace_##name(void (*probe)(proto))	\
	{								\
		return tracepoint_probe_unregister(#name, (void *)probe);\
	}

#else /* !CONFIG_TRACEPOINTS */
#undef DECLARE_TRACE
#define DECLARE_TRACE(name, proto, args)				\
	static inline void _do_trace_##name(struct tracepoint *tp, proto) \
	{ }								\
	static inline void trace_##name(proto)				\
	{ }								\
	static inline int register_trace_##name(void (*probe)(proto))	\
	{								\
		return -ENOSYS;						\
	}								\
	static inline int unregister_trace_##name(void (*probe)(proto))	\
	{								\
		return -ENOSYS;						\
	}
#endif /* CONFIG_TRACEPOINTS */

/* Define TRACE_EVENT for Declare tracepoint again*/

#undef TRACE_EVENT
#define TRACE_EVENT(name, proto, args, struct, assign, print)	\
	DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))
#undef TRACE_EVENT_FN
#define TRACE_EVENT_FN(name, proto, args, struct,		\
		assign, print, reg, unreg)			\
	DECLARE_TRACE(name, PARAMS(proto), PARAMS(args))
#endif /*_TRACEPOINT_DEFINE*/
#endif /* _LINUX_TRACEPOINT_H*/
