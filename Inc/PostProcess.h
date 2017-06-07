//--------------------------------------------------------------------------------------
// File: PostProcess.h
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------

#pragma once

#if defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
#error Post-processing not supported for Windows Phone 8.x; requires Direct3D hardware Feature Level 10.0 or better
#endif

#if defined(_XBOX_ONE) && defined(_TITLE)
#include <d3d12_x.h>
#else
#include <d3d12.h>
#endif

#include <DirectXMath.h>
#include <memory>
#include <functional>

#include "RenderTargetState.h"


namespace DirectX
{
    //----------------------------------------------------------------------------------
    // Abstract interface representing a post-process pass
    class IPostProcess
    {
    public:
        virtual ~IPostProcess() { }

        virtual void __cdecl Process(_In_ ID3D12GraphicsCommandList* commandList) = 0;
    };


    //----------------------------------------------------------------------------------
    // Basic post-process
    class BasicPostProcess : public IPostProcess
    {
    public:
        enum Effect
        {
            Copy,
            Monochrome,
            Sepia,
            DownScale_2x2,
            DownScale_4x4,
            GaussianBlur_5x5,
            BloomExtract,
            BloomBlur,
            Effect_Max
        };

        explicit BasicPostProcess(_In_ ID3D12Device* device, const RenderTargetState& rtState, Effect fx);
        BasicPostProcess(BasicPostProcess&& moveFrom);
        BasicPostProcess& operator= (BasicPostProcess&& moveFrom);

        BasicPostProcess(BasicPostProcess const&) = delete;
        BasicPostProcess& operator= (BasicPostProcess const&) = delete;

        virtual ~BasicPostProcess();

        // IPostProcess methods.
        void __cdecl Process(_In_ ID3D12GraphicsCommandList* commandList) override;

        // Properties
        void __cdecl SetSourceTexture(D3D12_GPU_DESCRIPTOR_HANDLE srvDescriptor, _In_opt_ ID3D12Resource* resource);

        // Sets multiplier for GaussianBlur_5x5
        void __cdecl SetGaussianParameter(float multiplier);

        // Sets parameters for BloomExtract
        void __cdecl SetBloomExtractParameter(float threshold);

        // Sets parameters for BloomBlur
        void __cdecl SetBloomBlurParameters(bool horizontal, float size, float brightness);

    private:
        // Private implementation.
        class Impl;

        std::unique_ptr<Impl> pImpl;
    };


    //----------------------------------------------------------------------------------
    // Dual-texure post-process
    class DualPostProcess : public IPostProcess
    {
    public:
        enum Effect
        {
            Merge,
            BloomCombine,
            Effect_Max
        };

        explicit DualPostProcess(_In_ ID3D12Device* device, const RenderTargetState& rtState, Effect fx);
        DualPostProcess(DualPostProcess&& moveFrom);
        DualPostProcess& operator= (DualPostProcess&& moveFrom);

        DualPostProcess(DualPostProcess const&) = delete;
        DualPostProcess& operator= (DualPostProcess const&) = delete;

        virtual ~DualPostProcess();

        // IPostProcess methods.
        void __cdecl Process(_In_ ID3D12GraphicsCommandList* commandList) override;

        // Properties
        void __cdecl SetSourceTexture(D3D12_GPU_DESCRIPTOR_HANDLE srvDescriptor);
        void __cdecl SetSourceTexture2(D3D12_GPU_DESCRIPTOR_HANDLE srvDescriptor);

        // Sets parameters for Merge
        void __cdecl SetMergeParameters(float weight1, float weight2);

        // Sets parameters for BloomCombine
        void __cdecl SetBloomCombineParameters(float bloom, float base, float bloomSaturation, float baseSaturation);

    private:
        // Private implementation.
        class Impl;

        std::unique_ptr<Impl> pImpl;
    };


    //----------------------------------------------------------------------------------
    // Tone-map post-process
    class ToneMapPostProcess : public IPostProcess
    {
    public:
        enum Effect
        {
            Saturate,
            Reinhard,
            Filmic,
            HDR10,
            HDR10_Saturate,
            HDR10_Reinhard,
            HDR10_Filmic,
            Effect_Max
        };

        explicit ToneMapPostProcess(_In_ ID3D12Device* device, const RenderTargetState& rtState, Effect fx);
        ToneMapPostProcess(ToneMapPostProcess&& moveFrom);
        ToneMapPostProcess& operator= (ToneMapPostProcess&& moveFrom);

        ToneMapPostProcess(ToneMapPostProcess const&) = delete;
        ToneMapPostProcess& operator= (ToneMapPostProcess const&) = delete;

        virtual ~ToneMapPostProcess();

        // IPostProcess methods.
        void __cdecl Process(_In_ ID3D12GraphicsCommandList* commandList) override;

        // Properties
        void __cdecl SetHDRSourceTexture(D3D12_GPU_DESCRIPTOR_HANDLE srvDescriptor);

        // Sets HDR10 display mapping parameter
        void SetHDR10Parameter(float paperWhiteNits);

    private:
        // Private implementation.
        class Impl;

        std::unique_ptr<Impl> pImpl;
    };
}