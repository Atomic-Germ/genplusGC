---
applyTo: **
description: Appendix of additional optimization and improvement opportunities not covered in genplusGC.instructions.md
---

# Genesis Plus GX GC/Wii Appendix: Additional Optimization Opportunities

Note: These items are not explicitly listed in the main instructions and target concrete wins for GC/Wii.

## Build and link optimizations
- Enable link-time optimization and dead code elimination for GC/Wii: add CFLAGS: -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables; LDFLAGS: -Wl,--gc-sections; consider -flto if supported by devkitPPC toolchain.
- For C++ GUI, compile with -fno-exceptions -fno-rtti and set -fvisibility=hidden to reduce binary size.
- Strip symbols in release builds: add -s to LDFLAGS and provide a debug profile that preserves symbols.
- PPC tuning flags audit: keep -mcpu=750 -mhard-float; evaluate -frename-registers and -fomit-frame-pointer (already used) for hot code only to avoid code bloat.

## Asset pipeline (remove runtime PNG decode paths)
- Preconvert GUI imagery to GPU-ready formats at build time to avoid libpng decode at runtime:
  - Prefer TPL or pre-swizzled RGB565 textures with correct GX tile order; generate .o via bin2o.
  - Remove or gate runtime libpng usage in gx_video.c for UI assets; keep only for screenshots if needed.

## Video path (GX) improvements
- Replace per-frame immediate primitive setup with prebuilt display lists for static quads/menus; only update matrices/texenv per frame.
- Evaluate using GX_CopyDisp/EFB-scaled copy for simple scale/present to cut draw overhead when no post-processing is active.
- Default to disabling NTSC filters (md_ntsc/sms_ntsc) on GameCube; expose a toggle. Convert filters to fixed-point/LUT where possible.
- Add optional 240p progressive mode (Wii + component) and safe 16:9 pillarbox with correct PAR/SAR; cache viewport/scissor for each mode.
- Ensure all dynamic textures and frame buffers are 32B aligned and in MEM2 on Wii; flush/invalidate once per frame, not per draw.

## Audio path improvements
- Make SOUND_BUFFER_NUM adaptive (2–4) based on measured DMA underruns via ai_callback; expose "low-latency" and "safe" presets.
- When vsync-locked, apply tighter dynamic rate control (smaller drift step) to reduce pitch modulation; keep GC/Wii in lockstep with VIDEO post-retrace callback.
- Place large audio work buffers in MEM2 (Wii) and avoid memcpy by writing directly into DMA-aligned buffers; batch DCStoreRange calls.

## CPU core selections (optional speed profiles)
- Offer alternative fast CPU cores behind build flags for GameCube-only speed profiles:
  - 68K: Cyclone (or another fast core) option vs current core for a "fast but less accurate" build.
  - Z80: DrZ80/CZ80 option for similar trade-off.
- Keep default as accuracy-first; document incompatibilities of fast cores.

## VDP renderer micro-optimizations
- Tighten inner pixel loops in core/vdp_render.c:
  - Process 8/16 pixels per iteration; use restrict pointers and precomputed line masks/palettes.
  - Hoist READ_LONG/WRITE_LONG branches out of hot paths when alignment is guaranteed; add likely/unlikely hints.
  - Precompute per-line sprite priority/LUTs to minimize conditionals in span renderers.

## File I/O and storage
- Implement directory listing cache in file browser; reuse stat() results and support incremental loading for large folders.
- Increase file I/O buffers to 32–64KB, 32B-aligned; stream CHD sectors with readahead instead of large one-shot reads.
- On Wii, allocate ROM buffers and CHD working sets from MEM2; add a tiny arena allocator wrapper to prefer MEM2 when available.

## Input and UI responsiveness
- Replace blocking controller-config loops (busy VIDEO_WaitVSync polling) with a non-blocking state machine so the GUI keeps updating.
- Add analog stick smoothing/deadzone calibration; cache mapping lookups to avoid string compares in hot paths.

## Save states and compression
- Switch to miniz/ozlib for faster state compression or expose an option for uncompressed states on GameCube to reduce CPU time.
- Defer thumbnail generation until after state write completes; run on next frame to avoid frame hitching.

## Conditional features and size trims
- Build-time switches to exclude: CHD/FLAC (GameCube), Wii DRC, OGG BGM, NTSC filters, screenshots; provide a "lite" target for small DOLs.
- Remove unused localization/assets from final builds; compress remaining assets with optimal zlib level offline.

## Diagnostics and profiling
- Add lightweight PPC timebase profiling macros around emulation/frame steps; expose an on-screen perf overlay for internal builds.
- Log throttled (ring buffer) to avoid I/O stalls; ensure LOGERROR is compiled out in release.

## Makefile.gc / Makefile.wii concrete changes (examples)
- CFLAGS += -ffunction-sections -fdata-sections -fno-asynchronous-unwind-tables
- LDFLAGS += -Wl,--gc-sections -s
- Optional (if toolchain supports): CFLAGS += -flto; LDFLAGS += -flto
- For C++ GUI files only: add -fno-exceptions -fno-rtti and -fvisibility=hidden
