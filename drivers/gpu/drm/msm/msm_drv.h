/*
 * Copyright (C) 2013 Red Hat
 * Author: Rob Clark <robdclark@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MSM_DRV_H__
#define __MSM_DRV_H__

#include <linux/kernel.h>
#include <linux/clk.h>
#include <linux/cpufreq.h>
#include <linux/module.h>
#include <linux/component.h>
#include <linux/platform_device.h>
#include <linux/pm.h>
#include <linux/pm_runtime.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/iommu.h>
#include <linux/types.h>
#include <linux/of_graph.h>
#include <linux/of_device.h>
#include <asm/sizes.h>

#include <drm/drmP.h>
#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_plane_helper.h>
#include <drm/drm_fb_helper.h>
#include <drm/msm_drm.h>
#include <drm/drm_gem.h>

struct msm_kms;
struct msm_gpu;
struct msm_mmu;
struct msm_mdss;
struct msm_rd_state;
struct msm_perf_state;
struct msm_gem_submit;
struct msm_fence_context;
struct msm_fence_cb;

#define NUM_DOMAINS 2    /* one for KMS, then one per gpu core (?) */

struct msm_file_private {
	/* currently we don't do anything useful with this.. but when
	 * per-context address spaces are supported we'd keep track of
	 * the context's page-tables here.
	 */
	int dummy;
};

enum msm_mdp_plane_property {
	PLANE_PROP_ZPOS,
	PLANE_PROP_ALPHA,
	PLANE_PROP_PREMULTIPLIED,
	PLANE_PROP_MAX_NUM
};

<<<<<<< HEAD
struct msm_vblank_ctrl {
	struct work_struct work;
	struct list_head event_list;
	spinlock_t lock;
=======
#define MAX_H_TILES_PER_DISPLAY 2

/**
 * enum msm_display_compression_type - compression method used for pixel stream
 * @MSM_DISPLAY_COMPRESSION_NONE:     Pixel data is not compressed
 * @MSM_DISPLAY_COMPRESSION_DSC:      DSC compresison is used
 */
enum msm_display_compression_type {
	MSM_DISPLAY_COMPRESSION_NONE,
	MSM_DISPLAY_COMPRESSION_DSC,
};

/**
 * enum msm_display_caps - features/capabilities supported by displays
 * @MSM_DISPLAY_CAP_VID_MODE:           Video or "active" mode supported
 * @MSM_DISPLAY_CAP_CMD_MODE:           Command mode supported
 * @MSM_DISPLAY_CAP_HOT_PLUG:           Hot plug detection supported
 * @MSM_DISPLAY_CAP_EDID:               EDID supported
 * @MSM_DISPLAY_ESD_ENABLED:            ESD feature enabled
 */
enum msm_display_caps {
	MSM_DISPLAY_CAP_VID_MODE	= BIT(0),
	MSM_DISPLAY_CAP_CMD_MODE	= BIT(1),
	MSM_DISPLAY_CAP_HOT_PLUG	= BIT(2),
	MSM_DISPLAY_CAP_EDID		= BIT(3),
	MSM_DISPLAY_ESD_ENABLED		= BIT(4),
};

/**
 * enum msm_event_wait - type of HW events to wait for
 * @MSM_ENC_COMMIT_DONE - wait for the driver to flush the registers to HW
 * @MSM_ENC_TX_COMPLETE - wait for the HW to transfer the frame to panel
 * @MSM_ENC_VBLANK - wait for the HW VBLANK event (for driver-internal waiters)
 * @MSM_ENC_ACTIVE_REGION - wait for the TG to be in active pixel region
 */
enum msm_event_wait {
	MSM_ENC_COMMIT_DONE = 0,
	MSM_ENC_TX_COMPLETE,
	MSM_ENC_VBLANK,
	MSM_ENC_ACTIVE_REGION,
};

/**
 * struct msm_roi_alignment - region of interest alignment restrictions
 * @xstart_pix_align: left x offset alignment restriction
 * @width_pix_align: width alignment restriction
 * @ystart_pix_align: top y offset alignment restriction
 * @height_pix_align: height alignment restriction
 * @min_width: minimum width restriction
 * @min_height: minimum height restriction
 */
struct msm_roi_alignment {
	uint32_t xstart_pix_align;
	uint32_t width_pix_align;
	uint32_t ystart_pix_align;
	uint32_t height_pix_align;
	uint32_t min_width;
	uint32_t min_height;
};

/**
 * struct msm_roi_caps - display's region of interest capabilities
 * @enabled: true if some region of interest is supported
 * @merge_rois: merge rois before sending to display
 * @num_roi: maximum number of rois supported
 * @align: roi alignment restrictions
 */
struct msm_roi_caps {
	bool enabled;
	bool merge_rois;
	uint32_t num_roi;
	struct msm_roi_alignment align;
};

/**
 * struct msm_display_dsc_info - defines dsc configuration
 * @version:                 DSC version.
 * @scr_rev:                 DSC revision.
 * @pic_height:              Picture height in pixels.
 * @pic_width:               Picture width in pixels.
 * @initial_lines:           Number of initial lines stored in encoder.
 * @pkt_per_line:            Number of packets per line.
 * @bytes_in_slice:          Number of bytes in slice.
 * @eol_byte_num:            Valid bytes at the end of line.
 * @pclk_per_line:           Compressed width.
 * @full_frame_slices:       Number of slice per interface.
 * @slice_height:            Slice height in pixels.
 * @slice_width:             Slice width in pixels.
 * @chunk_size:              Chunk size in bytes for slice multiplexing.
 * @slice_last_group_size:   Size of last group in pixels.
 * @bpp:                     Target bits per pixel.
 * @bpc:                     Number of bits per component.
 * @line_buf_depth:          Line buffer bit depth.
 * @block_pred_enable:       Block prediction enabled/disabled.
 * @vbr_enable:              VBR mode.
 * @enable_422:              Indicates if input uses 4:2:2 sampling.
 * @convert_rgb:             DSC color space conversion.
 * @input_10_bits:           10 bit per component input.
 * @slice_per_pkt:           Number of slices per packet.
 * @initial_dec_delay:       Initial decoding delay.
 * @initial_xmit_delay:      Initial transmission delay.
 * @initial_scale_value:     Scale factor value at the beginning of a slice.
 * @scale_decrement_interval: Scale set up at the beginning of a slice.
 * @scale_increment_interval: Scale set up at the end of a slice.
 * @first_line_bpg_offset:   Extra bits allocated on the first line of a slice.
 * @nfl_bpg_offset:          Slice specific settings.
 * @slice_bpg_offset:        Slice specific settings.
 * @initial_offset:          Initial offset at the start of a slice.
 * @final_offset:            Maximum end-of-slice value.
 * @rc_model_size:           Number of bits in RC model.
 * @det_thresh_flatness:     Flatness threshold.
 * @max_qp_flatness:         Maximum QP for flatness adjustment.
 * @min_qp_flatness:         Minimum QP for flatness adjustment.
 * @edge_factor:             Ratio to detect presence of edge.
 * @quant_incr_limit0:       QP threshold.
 * @quant_incr_limit1:       QP threshold.
 * @tgt_offset_hi:           Upper end of variability range.
 * @tgt_offset_lo:           Lower end of variability range.
 * @buf_thresh:              Thresholds in RC model
 * @range_min_qp:            Min QP allowed.
 * @range_max_qp:            Max QP allowed.
 * @range_bpg_offset:        Bits per group adjustment.
 */
struct msm_display_dsc_info {
	u8 version;
	u8 scr_rev;

	int pic_height;
	int pic_width;
	int slice_height;
	int slice_width;

	int initial_lines;
	int pkt_per_line;
	int bytes_in_slice;
	int bytes_per_pkt;
	int eol_byte_num;
	int pclk_per_line;
	int full_frame_slices;
	int slice_last_group_size;
	int bpp;
	int bpc;
	int line_buf_depth;

	int slice_per_pkt;
	int chunk_size;
	bool block_pred_enable;
	int vbr_enable;
	int enable_422;
	int convert_rgb;
	int input_10_bits;

	int initial_dec_delay;
	int initial_xmit_delay;
	int initial_scale_value;
	int scale_decrement_interval;
	int scale_increment_interval;
	int first_line_bpg_offset;
	int nfl_bpg_offset;
	int slice_bpg_offset;
	int initial_offset;
	int final_offset;

	int rc_model_size;
	int det_thresh_flatness;
	int max_qp_flatness;
	int min_qp_flatness;
	int edge_factor;
	int quant_incr_limit0;
	int quant_incr_limit1;
	int tgt_offset_hi;
	int tgt_offset_lo;

	u32 *buf_thresh;
	char *range_min_qp;
	char *range_max_qp;
	char *range_bpg_offset;
};

/**
 * struct msm_compression_info - defined panel compression
 * @comp_type:        type of compression supported
 * @dsc_info:         dsc configuration if the compression
 *                    supported is DSC
 */
struct msm_compression_info {
	enum msm_display_compression_type comp_type;

	union{
		struct msm_display_dsc_info dsc_info;
	};
};

/**
 * struct msm_display_topology - defines a display topology pipeline
 * @num_lm:       number of layer mixers used
 * @num_enc:      number of compression encoder blocks used
 * @num_intf:     number of interfaces the panel is mounted on
 */
struct msm_display_topology {
	u32 num_lm;
	u32 num_enc;
	u32 num_intf;
};

/**
 * struct msm_mode_info - defines all msm custom mode info
 * @frame_rate:      frame_rate of the mode
 * @vtotal:          vtotal calculated for the mode
 * @prefill_lines:   prefill lines based on porches.
 * @jitter_numer:	display panel jitter numerator configuration
 * @jitter_denom:	display panel jitter denominator configuration
 * @clk_rate:	     DSI bit clock per lane in HZ.
 * @topology:        supported topology for the mode
 * @comp_info:       compression info supported
 * @roi_caps:        panel roi capabilities
 */
struct msm_mode_info {
	uint32_t frame_rate;
	uint32_t vtotal;
	uint32_t prefill_lines;
	uint32_t jitter_numer;
	uint32_t jitter_denom;
	uint64_t clk_rate;
	struct msm_display_topology topology;
	struct msm_compression_info comp_info;
	struct msm_roi_caps roi_caps;
};

/**
 * struct msm_display_info - defines display properties
 * @intf_type:          DRM_MODE_CONNECTOR_ display type
 * @capabilities:       Bitmask of display flags
 * @num_of_h_tiles:     Number of horizontal tiles in case of split interface
 * @h_tile_instance:    Controller instance used per tile. Number of elements is
 *                      based on num_of_h_tiles
 * @is_connected:       Set to true if display is connected
 * @width_mm:           Physical width
 * @height_mm:          Physical height
 * @max_width:          Max width of display. In case of hot pluggable display
 *                      this is max width supported by controller
 * @max_height:         Max height of display. In case of hot pluggable display
 *                      this is max height supported by controller
 * @clk_rate:           DSI bit clock per lane in HZ.
 * @is_primary:         Set to true if display is primary display
 * @is_te_using_watchdog_timer:  Boolean to indicate watchdog TE is
 *				 used instead of panel TE in cmd mode panels
 * @roi_caps:           Region of interest capability info
 */
struct msm_display_info {
	int intf_type;
	uint32_t capabilities;

	uint32_t num_of_h_tiles;
	uint32_t h_tile_instance[MAX_H_TILES_PER_DISPLAY];

	bool is_connected;

	unsigned int width_mm;
	unsigned int height_mm;

	uint32_t max_width;
	uint32_t max_height;
	uint64_t clk_rate;

	bool is_primary;
	bool is_te_using_watchdog_timer;
	struct msm_roi_caps roi_caps;
};

#define MSM_MAX_ROI	4

/**
 * struct msm_roi_list - list of regions of interest for a drm object
 * @num_rects: number of valid rectangles in the roi array
 * @roi: list of roi rectangles
 */
struct msm_roi_list {
	uint32_t num_rects;
	struct drm_clip_rect roi[MSM_MAX_ROI];
};

/**
 * struct - msm_display_kickoff_params - info for display features at kickoff
 * @rois: Regions of interest structure for mapping CRTC to Connector output
 */
struct msm_display_kickoff_params {
	struct msm_roi_list *rois;
	struct drm_msm_ext_hdr_metadata *hdr_meta;
};

/**
 * struct msm_drm_event - defines custom event notification struct
 * @base: base object required for event notification by DRM framework.
 * @event: event object required for event notification by DRM framework.
 * @info: contains information of DRM object for which events has been
 *        requested.
 * @data: memory location which contains response payload for event.
 */
struct msm_drm_event {
	struct drm_pending_event base;
	struct drm_event event;
	struct drm_msm_event_req info;
	u8 data[];
};

/* Commit/Event thread specific structure */
struct msm_drm_thread {
	struct drm_device *dev;
	struct task_struct *thread;
	unsigned int crtc_id;
	struct kthread_worker worker;
};

struct msm_idle {
	u32 timeout_ms;
	u32 encoder_mask;
	u32 active_mask;

	spinlock_t lock;
	struct delayed_work work;
>>>>>>> b0c3bbac1b16... Merge branch 'freak07-caf'
};

struct msm_drm_private {

	struct drm_device *dev;

	struct msm_kms *kms;

	/* subordinate devices, if present: */
	struct platform_device *gpu_pdev;

	/* top level MDSS wrapper device (for MDP5 only) */
	struct msm_mdss *mdss;

	/* possibly this should be in the kms component, but it is
	 * shared by both mdp4 and mdp5..
	 */
	struct hdmi *hdmi;

	/* eDP is for mdp5 only, but kms has not been created
	 * when edp_bind() and edp_init() are called. Here is the only
	 * place to keep the edp instance.
	 */
	struct msm_edp *edp;

	/* DSI is shared by mdp4 and mdp5 */
	struct msm_dsi *dsi[2];

	/* when we have more than one 'msm_gpu' these need to be an array: */
	struct msm_gpu *gpu;
	struct msm_file_private *lastctx;

	struct drm_fb_helper *fbdev;

	struct msm_rd_state *rd;
	struct msm_perf_state *perf;

	/* list of GEM objects: */
	struct list_head inactive_list;

	struct workqueue_struct *wq;
	struct workqueue_struct *atomic_wq;

	/* crtcs pending async atomic updates: */
	uint32_t pending_crtcs;
	wait_queue_head_t pending_crtcs_event;

	/* registered MMUs: */
	unsigned int num_mmus;
	struct msm_mmu *mmus[NUM_DOMAINS];

	unsigned int num_planes;
	struct drm_plane *planes[8];

	unsigned int num_crtcs;
	struct drm_crtc *crtcs[8];

	unsigned int num_encoders;
	struct drm_encoder *encoders[8];

	unsigned int num_bridges;
	struct drm_bridge *bridges[8];

	unsigned int num_connectors;
	struct drm_connector *connectors[8];

	/* Properties */
	struct drm_property *plane_property[PLANE_PROP_MAX_NUM];

	/* VRAM carveout, used when no IOMMU: */
	struct {
		unsigned long size;
		dma_addr_t paddr;
		/* NOTE: mm managed at the page level, size is in # of pages
		 * and position mm_node->start is in # of pages:
		 */
		struct drm_mm mm;
	} vram;

	struct notifier_block vmap_notifier;
	struct shrinker shrinker;

	/* task holding struct_mutex.. currently only used in submit path
	 * to detect and reject faults from copy_from_user() for submit
	 * ioctl.
	 */
	struct task_struct *struct_mutex_task;
};

struct msm_format {
	uint32_t pixel_format;
};

int msm_atomic_check(struct drm_device *dev,
		     struct drm_atomic_state *state);
int msm_atomic_commit(struct drm_device *dev,
		struct drm_atomic_state *state, bool nonblock);

int msm_register_mmu(struct drm_device *dev, struct msm_mmu *mmu);

void msm_gem_submit_free(struct msm_gem_submit *submit);
int msm_ioctl_gem_submit(struct drm_device *dev, void *data,
		struct drm_file *file);

void msm_gem_shrinker_init(struct drm_device *dev);
void msm_gem_shrinker_cleanup(struct drm_device *dev);

int msm_gem_mmap_obj(struct drm_gem_object *obj,
			struct vm_area_struct *vma);
int msm_gem_mmap(struct file *filp, struct vm_area_struct *vma);
int msm_gem_fault(struct vm_area_struct *vma, struct vm_fault *vmf);
uint64_t msm_gem_mmap_offset(struct drm_gem_object *obj);
int msm_gem_get_iova_locked(struct drm_gem_object *obj, int id,
		uint32_t *iova);
int msm_gem_get_iova(struct drm_gem_object *obj, int id, uint32_t *iova);
uint32_t msm_gem_iova(struct drm_gem_object *obj, int id);
struct page **msm_gem_get_pages(struct drm_gem_object *obj);
void msm_gem_put_pages(struct drm_gem_object *obj);
void msm_gem_put_iova(struct drm_gem_object *obj, int id);
int msm_gem_dumb_create(struct drm_file *file, struct drm_device *dev,
		struct drm_mode_create_dumb *args);
int msm_gem_dumb_map_offset(struct drm_file *file, struct drm_device *dev,
		uint32_t handle, uint64_t *offset);
struct sg_table *msm_gem_prime_get_sg_table(struct drm_gem_object *obj);
void *msm_gem_prime_vmap(struct drm_gem_object *obj);
void msm_gem_prime_vunmap(struct drm_gem_object *obj, void *vaddr);
int msm_gem_prime_mmap(struct drm_gem_object *obj, struct vm_area_struct *vma);
struct reservation_object *msm_gem_prime_res_obj(struct drm_gem_object *obj);
struct drm_gem_object *msm_gem_prime_import_sg_table(struct drm_device *dev,
		struct dma_buf_attachment *attach, struct sg_table *sg);
int msm_gem_prime_pin(struct drm_gem_object *obj);
void msm_gem_prime_unpin(struct drm_gem_object *obj);
void *msm_gem_get_vaddr_locked(struct drm_gem_object *obj);
void *msm_gem_get_vaddr(struct drm_gem_object *obj);
void msm_gem_put_vaddr_locked(struct drm_gem_object *obj);
void msm_gem_put_vaddr(struct drm_gem_object *obj);
int msm_gem_madvise(struct drm_gem_object *obj, unsigned madv);
void msm_gem_purge(struct drm_gem_object *obj);
void msm_gem_vunmap(struct drm_gem_object *obj);
int msm_gem_sync_object(struct drm_gem_object *obj,
		struct msm_fence_context *fctx, bool exclusive);
void msm_gem_move_to_active(struct drm_gem_object *obj,
		struct msm_gpu *gpu, bool exclusive, struct fence *fence);
void msm_gem_move_to_inactive(struct drm_gem_object *obj);
int msm_gem_cpu_prep(struct drm_gem_object *obj, uint32_t op, ktime_t *timeout);
int msm_gem_cpu_fini(struct drm_gem_object *obj);
void msm_gem_free_object(struct drm_gem_object *obj);
int msm_gem_new_handle(struct drm_device *dev, struct drm_file *file,
		uint32_t size, uint32_t flags, uint32_t *handle);
struct drm_gem_object *msm_gem_new(struct drm_device *dev,
		uint32_t size, uint32_t flags);
struct drm_gem_object *msm_gem_import(struct drm_device *dev,
		struct dma_buf *dmabuf, struct sg_table *sgt);

int msm_framebuffer_prepare(struct drm_framebuffer *fb, int id);
void msm_framebuffer_cleanup(struct drm_framebuffer *fb, int id);
uint32_t msm_framebuffer_iova(struct drm_framebuffer *fb, int id, int plane);
struct drm_gem_object *msm_framebuffer_bo(struct drm_framebuffer *fb, int plane);
const struct msm_format *msm_framebuffer_format(struct drm_framebuffer *fb);
struct drm_framebuffer *msm_framebuffer_init(struct drm_device *dev,
		const struct drm_mode_fb_cmd2 *mode_cmd, struct drm_gem_object **bos);
struct drm_framebuffer *msm_framebuffer_create(struct drm_device *dev,
		struct drm_file *file, const struct drm_mode_fb_cmd2 *mode_cmd);

struct drm_fb_helper *msm_fbdev_init(struct drm_device *dev);
void msm_fbdev_free(struct drm_device *dev);

struct hdmi;
int msm_hdmi_modeset_init(struct hdmi *hdmi, struct drm_device *dev,
		struct drm_encoder *encoder);
void __init msm_hdmi_register(void);
void __exit msm_hdmi_unregister(void);

struct msm_edp;
void __init msm_edp_register(void);
void __exit msm_edp_unregister(void);
int msm_edp_modeset_init(struct msm_edp *edp, struct drm_device *dev,
		struct drm_encoder *encoder);

struct msm_dsi;
enum msm_dsi_encoder_id {
	MSM_DSI_VIDEO_ENCODER_ID = 0,
	MSM_DSI_CMD_ENCODER_ID = 1,
	MSM_DSI_ENCODER_NUM = 2
};
#ifdef CONFIG_DRM_MSM_DSI
void __init msm_dsi_register(void);
void __exit msm_dsi_unregister(void);
int msm_dsi_modeset_init(struct msm_dsi *msm_dsi, struct drm_device *dev,
		struct drm_encoder *encoders[MSM_DSI_ENCODER_NUM]);
#else
static inline void __init msm_dsi_register(void)
{
}
static inline void __exit msm_dsi_unregister(void)
{
}
static inline int msm_dsi_modeset_init(struct msm_dsi *msm_dsi,
		struct drm_device *dev,
		struct drm_encoder *encoders[MSM_DSI_ENCODER_NUM])
{
	return -EINVAL;
}
#endif

void __init msm_mdp_register(void);
void __exit msm_mdp_unregister(void);

#ifdef CONFIG_DEBUG_FS
void msm_gem_describe(struct drm_gem_object *obj, struct seq_file *m);
void msm_gem_describe_objects(struct list_head *list, struct seq_file *m);
void msm_framebuffer_describe(struct drm_framebuffer *fb, struct seq_file *m);
int msm_debugfs_late_init(struct drm_device *dev);
int msm_rd_debugfs_init(struct drm_minor *minor);
void msm_rd_debugfs_cleanup(struct drm_minor *minor);
void msm_rd_dump_submit(struct msm_gem_submit *submit);
int msm_perf_debugfs_init(struct drm_minor *minor);
void msm_perf_debugfs_cleanup(struct drm_minor *minor);
#else
static inline int msm_debugfs_late_init(struct drm_device *dev) { return 0; }
static inline void msm_rd_dump_submit(struct msm_gem_submit *submit) {}
#endif

void __iomem *msm_ioremap(struct platform_device *pdev, const char *name,
		const char *dbgname);
void msm_writel(u32 data, void __iomem *addr);
u32 msm_readl(const void __iomem *addr);

#define DBG(fmt, ...) DRM_DEBUG(fmt"\n", ##__VA_ARGS__)
#define VERB(fmt, ...) if (0) DRM_DEBUG(fmt"\n", ##__VA_ARGS__)

static inline int align_pitch(int width, int bpp)
{
	int bytespp = (bpp + 7) / 8;
	/* adreno needs pitch aligned to 32 pixels: */
	return bytespp * ALIGN(width, 32);
}

/* for the generated headers: */
#define INVALID_IDX(idx) ({BUG(); 0;})
#define fui(x)                ({BUG(); 0;})
#define util_float_to_half(x) ({BUG(); 0;})


#define FIELD(val, name) (((val) & name ## __MASK) >> name ## __SHIFT)

/* for conditionally setting boolean flag(s): */
#define COND(bool, val) ((bool) ? (val) : 0)

static inline unsigned long timeout_to_jiffies(const ktime_t *timeout)
{
	ktime_t now = ktime_get();
	unsigned long remaining_jiffies;

	if (ktime_compare(*timeout, now) < 0) {
		remaining_jiffies = 0;
	} else {
		ktime_t rem = ktime_sub(*timeout, now);
		struct timespec ts = ktime_to_timespec(rem);
		remaining_jiffies = timespec_to_jiffies(&ts);
	}

	return remaining_jiffies;
}

#endif /* __MSM_DRV_H__ */
