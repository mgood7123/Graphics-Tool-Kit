//
// Created by Matthew Good on 4/8/21.
//

#ifndef GRAPHICAL_TOOL_KIT_PIPELINEMANAGER_H
#define GRAPHICAL_TOOL_KIT_PIPELINEMANAGER_H

#include <MultiTouch/Utils/AutoResizingArray.h>
#include <DiligentCore/Graphics/GraphicsEngine/interface/DeviceContext.h>
#include <DiligentCore/Graphics/GraphicsEngine/interface/RenderDevice.h>
#include <DiligentCore/Common/interface/RefCntAutoPtr.hpp>
#include <vector>
#include <WINAPI_KERNEL/SDK/include/Windows/Kernel/WindowsAPIKernel.h>

// On CPU side OpenGL is approximately 10 times slower than other backends.
// On GPU it may be as fast, depending on your application

class PipelineManager {
public:
    struct Key {
    public:
        std::string key;
        inline Key() {
            key = "";
        }
        inline Key(const std::string & key) {
            this->key = key;
        }
    };
    
    class PipelineObject {
        Diligent::GraphicsPipelineStateCreateInfo pipelineStateCreateInfo;
        char * c_internal_key;
        size_t keyLen;
        Diligent::RefCntAutoPtr<Diligent::IPipelineState> state;
        Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> binding;

        std::vector<Diligent::LayoutElement> inputLayoutElements;
        std::vector<Diligent::ImmutableSamplerDesc> resourceImmutableSamplers;
        std::vector<Diligent::ShaderResourceVariableDesc> resourceVariables;
    public:
        PipelineObject();
        PipelineObject(void * instance, const char * name);
        
        inline PipelineObject (const PipelineObject & obj) {
            auto & p = const_cast<PipelineObject&>(obj);
            pipelineStateCreateInfo = std::move(p.pipelineStateCreateInfo);
            c_internal_key = p.c_internal_key;
            p.c_internal_key = nullptr;
            keyLen = p.keyLen;
            p.keyLen = 0;
            state = std::move(p.state);
            binding = std::move(p.binding);
            inputLayoutElements = std::move(p.inputLayoutElements);
            resourceImmutableSamplers = std::move(p.resourceImmutableSamplers);
            resourceVariables = std::move(p.resourceVariables);
        }

        inline PipelineObject (PipelineObject && obj) {
            pipelineStateCreateInfo = std::move(obj.pipelineStateCreateInfo);
            c_internal_key = obj.c_internal_key;
            obj.c_internal_key = nullptr;
            keyLen = obj.keyLen;
            obj.keyLen = 0;
            state = std::move(obj.state);
            binding = std::move(obj.binding);
            inputLayoutElements = std::move(obj.inputLayoutElements);
            resourceImmutableSamplers = std::move(obj.resourceImmutableSamplers);
            resourceVariables = std::move(obj.resourceVariables);
        }

        inline PipelineObject & operator= (const PipelineObject & obj) {
            auto & p = const_cast<PipelineObject&>(obj);
            pipelineStateCreateInfo = std::move(p.pipelineStateCreateInfo);
            c_internal_key = p.c_internal_key;
            p.c_internal_key = nullptr;
            keyLen = p.keyLen;
            p.keyLen = 0;
            state = std::move(p.state);
            binding = std::move(p.binding);
            inputLayoutElements = std::move(p.inputLayoutElements);
            resourceImmutableSamplers = std::move(p.resourceImmutableSamplers);
            resourceVariables = std::move(p.resourceVariables);
            return *this;
        }

        inline PipelineObject & operator= (PipelineObject && obj) {
            pipelineStateCreateInfo = std::move(obj.pipelineStateCreateInfo);
            c_internal_key = obj.c_internal_key;
            obj.c_internal_key = nullptr;
            keyLen = obj.keyLen;
            obj.keyLen = 0;
            state = std::move(obj.state);
            binding = std::move(obj.binding);
            inputLayoutElements = std::move(obj.inputLayoutElements);
            resourceImmutableSamplers = std::move(obj.resourceImmutableSamplers);
            resourceVariables = std::move(obj.resourceVariables);
            return *this;
        }

        ~PipelineObject();

        const Diligent::GraphicsPipelineStateCreateInfo &getPipelineStateCreateInfo() const;

        Diligent::RefCntAutoPtr<Diligent::IPipelineState> &getState();

        Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> &getBinding();

        void setType(const Diligent::PIPELINE_TYPE & pipelineType);

        void setFormat(Diligent::ISwapChain *pChain);

        void setNumberOfTargets(int numberOfTargets);

        void setPrimitiveTopology(const Diligent::PRIMITIVE_TOPOLOGY &topology);

        void setCullMode(const Diligent::CULL_MODE &cullMode);

        void setDepthTesting(bool testForDepth);
        
        void setScissorEnable(bool scissorEnable);

        void setInputLayout(const std::vector<Diligent::LayoutElement> & layoutElements);

        void setResourceLayoutSamplers(
                const std::vector<Diligent::ImmutableSamplerDesc> & immutableSamplers
        );

        void setResourceLayoutVariables(
                const std::vector<Diligent::ShaderResourceVariableDesc> & variables
        );

        void setShaders(Diligent::IShader *vertexShader, Diligent::IShader *pixelShader);

        void
        createPipelineState(Diligent::IRenderDevice *pDevice);

        void
        createShaderBinding(bool initStaticResources);

        Diligent::IShaderResourceVariable *
        getStaticVariableFromShader(Diligent::SHADER_TYPE shaderType, const char *name);

        Diligent::IShaderResourceVariable *
        getVariableFromShader(Diligent::SHADER_TYPE shaderType, const char *name);

        Diligent::IShaderResourceVariable * getStaticVariableFromVertexShader(const char *name);

        Diligent::IShaderResourceVariable * getVariableFromVertexShader(const char *name);

        Diligent::IShaderResourceVariable * getStaticVariableFromPixelShader(const char *name);

        Diligent::IShaderResourceVariable * getVariableFromPixelShader(const char *name);

        const char *getName();

        bool keyMatches(const Key & key);
        bool keyMatches(void * ptr, const char *key);

        void setDefaultResourceLayoutVariableType(Diligent::SHADER_RESOURCE_VARIABLE_TYPE type);
    };

private:
    Kernel PSOs;
    size_t pipelines;
public:
    
    PipelineManager();
    ~PipelineManager();
    
    static Key make_key(void * ptr, const char * key);
    
    PipelineObject & createPipeline(void * instance, const char * key);

    std::pair<PipelineObject *, size_t> findPipeline(void * instance, const char *key);

    void switchToPipeline(
            void * instance, const char *key, Diligent::IDeviceContext *deviceContext
    );

    void switchToPipeline(
            PipelineObject & pipelineObject, Diligent::IDeviceContext *deviceContext
    );

    void commitShaderResourceBinding(
            void * instance, const char *key, Diligent::IDeviceContext *deviceContext,
            Diligent::RESOURCE_STATE_TRANSITION_MODE resourceStateTransitionMode
    );

    void commitShaderResourceBinding(
            PipelineObject & pipelineObject, Diligent::IDeviceContext *deviceContext,
            Diligent::RESOURCE_STATE_TRANSITION_MODE resourceStateTransitionMode
    );
    
    void switchToPipelineAndCommitShaderResourceBinding(
            void * instance, const char *key, Diligent::IDeviceContext *deviceContext,
            Diligent::RESOURCE_STATE_TRANSITION_MODE resourceStateTransitionMode
    );

    void switchToPipelineAndCommitShaderResourceBinding(
            PipelineObject &pipelineObject, Diligent::IDeviceContext *deviceContext,
            Diligent::RESOURCE_STATE_TRANSITION_MODE resourceStateTransitionMode
    );

    void destroyPipeline(void * instance, const char *key);
};


#endif //GRAPHICAL_TOOL_KIT_PIPELINEMANAGER_H
