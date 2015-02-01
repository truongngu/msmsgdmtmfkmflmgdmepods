#pragma once
#include "DebugDefine.h"
#if defined WindowPhone
#include <dxgiformat.h>
#include "Direct3DBase.h"
//--------------------------------------------------------------------------------------
// Macros
//--------------------------------------------------------------------------------------
#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
	((uint32)(byte)(ch0) | ((uint32)(byte)(ch1) << 8) | \
	((uint32)(byte)(ch2) << 16) | ((uint32)(byte)(ch3) << 24))
#endif /* defined(MAKEFOURCC) */
#pragma pack(push, 1)

#define DDS_MAGIC 0x20534444 // "DDS "

struct DDS_PIXELFORMAT
{
	uint32  size;
	uint32  flags;
	uint32  fourCC;
	uint32  RGBBitCount;
	uint32  RBitMask;
	uint32  GBitMask;
	uint32  BBitMask;
	uint32  ABitMask;
};

#define DDS_FOURCC      0x00000004  // DDPF_FOURCC
#define DDS_RGB         0x00000040  // DDPF_RGB
#define DDS_RGBA        0x00000041  // DDPF_RGB | DDPF_ALPHAPIXELS
#define DDS_LUMINANCE   0x00020000  // DDPF_LUMINANCE
#define DDS_LUMINANCEA  0x00020001  // DDPF_LUMINANCE | DDPF_ALPHAPIXELS
#define DDS_ALPHA       0x00000002  // DDPF_ALPHA
#define DDS_PAL8        0x00000020  // DDPF_PALETTEINDEXED8

#define DDS_HEADER_FLAGS_TEXTURE        0x00001007  // DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT
#define DDS_HEADER_FLAGS_MIPMAP         0x00020000  // DDSD_MIPMAPCOUNT
#define DDS_HEADER_FLAGS_VOLUME         0x00800000  // DDSD_DEPTH
#define DDS_HEADER_FLAGS_PITCH          0x00000008  // DDSD_PITCH
#define DDS_HEADER_FLAGS_LINEARSIZE     0x00080000  // DDSD_LINEARSIZE

#define DDS_HEIGHT 0x00000002 // DDSD_HEIGHT
#define DDS_WIDTH  0x00000004 // DDSD_WIDTH

#define DDS_SURFACE_FLAGS_TEXTURE 0x00001000 // DDSCAPS_TEXTURE
#define DDS_SURFACE_FLAGS_MIPMAP  0x00400008 // DDSCAPS_COMPLEX | DDSCAPS_MIPMAP
#define DDS_SURFACE_FLAGS_CUBEMAP 0x00000008 // DDSCAPS_COMPLEX

#define DDS_CUBEMAP_POSITIVEX 0x00000600 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
#define DDS_CUBEMAP_NEGATIVEX 0x00000a00 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
#define DDS_CUBEMAP_POSITIVEY 0x00001200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
#define DDS_CUBEMAP_NEGATIVEY 0x00002200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
#define DDS_CUBEMAP_POSITIVEZ 0x00004200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
#define DDS_CUBEMAP_NEGATIVEZ 0x00008200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ

#define DDS_CUBEMAP_ALLFACES (DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX |\
	DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY | \
	DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ)

#define DDS_CUBEMAP 0x00000200 // DDSCAPS2_CUBEMAP

#define DDS_FLAGS_VOLUME 0x00200000 // DDSCAPS2_VOLUME

typedef struct
{
	uint32          size;
	uint32          flags;
	uint32          height;
	uint32          width;
	uint32          pitchOrLinearSize;
	uint32          depth; // only if DDS_HEADER_FLAGS_VOLUME is set in flags
	uint32          mipMapCount;
	uint32          reserved1[11];
	DDS_PIXELFORMAT ddspf;
	uint32          caps;
	uint32          caps2;
	uint32          caps3;
	uint32          caps4;
	uint32          reserved2;
} DDS_HEADER;

typedef struct
{
	DXGI_FORMAT dxgiFormat;
	uint32      resourceDimension;
	uint32      miscFlag; // see D3D11_RESOURCE_MISC_FLAG
	uint32      arraySize;
	uint32      reserved;
} DDS_HEADER_DXT10;

#pragma pack(pop)

char * LoadDDS(const char * szFileName,
	_In_ ID3D11Device* d3dDevice,
	_Out_opt_ ID3D11Resource** texture,
	_Out_opt_ ID3D11ShaderResourceView** textureView);

void CreateDDSTextureFromMemory(
	_In_ ID3D11Device* d3dDevice,
	_In_reads_bytes_(ddsDataSize) const byte* ddsData,
	_In_ size_t ddsDataSize,
	_Out_opt_ ID3D11Resource** texture,
	_Out_opt_ ID3D11ShaderResourceView** textureView,
	_In_ size_t maxsize
	);
#endif