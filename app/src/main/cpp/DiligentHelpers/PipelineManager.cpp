//
// Created by Matthew Good on 4/8/21.
//

#include "PipelineManager.h"

PipelineManager::PipelineObject::PipelineObject(): PipelineObject(nullptr) {
}

PipelineManager::PipelineObject::PipelineObject(const char *key) {
    pipelineStateCreateInfo.PSODesc.Name = key;
    keyLen = key == nullptr ? 0 : strlen(key);

}

Diligent::RefCntAutoPtr<Diligent::IPipelineState> &PipelineManager::PipelineObject::getState() {
    return state;
}

Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> &PipelineManager::PipelineObject::getBinding() {
    return binding;
}

void PipelineManager::PipelineObject::setType(const Diligent::PIPELINE_TYPE &pipelineType) {
    pipelineStateCreateInfo.PSODesc.PipelineType = pipelineType;
}

void PipelineManager::PipelineObject::setNumberOfTargets(int numberOfTargets) {
    pipelineStateCreateInfo.GraphicsPipeline.NumRenderTargets = numberOfTargets;
}

void PipelineManager::PipelineObject::setFormat(Diligent::ISwapChain *pChain) {
    const Diligent::SwapChainDesc & desc = pChain->GetDesc();
    pipelineStateCreateInfo.GraphicsPipeline.RTVFormats[0] = desc.ColorBufferFormat;
    pipelineStateCreateInfo.GraphicsPipeline.DSVFormat = desc.DepthBufferFormat;
}

void PipelineManager::PipelineObject::setPrimitiveTopology(
        const Diligent::PRIMITIVE_TOPOLOGY &topology
) {
    pipelineStateCreateInfo.GraphicsPipeline.PrimitiveTopology = topology;
}

void PipelineManager::PipelineObject::setCullMode(const Diligent::CULL_MODE &cullMode) {
    pipelineStateCreateInfo.GraphicsPipeline.RasterizerDesc.CullMode = cullMode;
}

void PipelineManager::PipelineObject::setDepthTesting(bool testForDepth) {
    pipelineStateCreateInfo.GraphicsPipeline.DepthStencilDesc.DepthEnable = testForDepth;
}

void PipelineManager::PipelineObject::setScissorEnable(bool scissorEnable) {
    pipelineStateCreateInfo.GraphicsPipeline.RasterizerDesc.ScissorEnable = scissorEnable;
}

void
PipelineManager::PipelineObject::setInputLayout(
        const std::vector<Diligent::LayoutElement> & layoutElements
) {
    inputLayoutElements = layoutElements;
    pipelineStateCreateInfo.GraphicsPipeline.InputLayout.LayoutElements = inputLayoutElements.data();
    pipelineStateCreateInfo.GraphicsPipeline.InputLayout.NumElements = inputLayoutElements.size();
}

void
PipelineManager::PipelineObject::setResourceLayoutSamplers(
        const std::vector<Diligent::ImmutableSamplerDesc> & immutableSamplers
) {
    resourceImmutableSamplers = immutableSamplers;
    pipelineStateCreateInfo.PSODesc.ResourceLayout.ImmutableSamplers = resourceImmutableSamplers.data();
    pipelineStateCreateInfo.PSODesc.ResourceLayout.NumImmutableSamplers = resourceImmutableSamplers.size();
}

void PipelineManager::PipelineObject::setResourceLayoutVariables(
        const std::vector<Diligent::ShaderResourceVariableDesc> & variables
) {
    resourceVariables = variables;
    pipelineStateCreateInfo.PSODesc.ResourceLayout.Variables = resourceVariables.data();
    pipelineStateCreateInfo.PSODesc.ResourceLayout.NumVariables = resourceVariables.size();
}

void PipelineManager::PipelineObject::setShaders(Diligent::IShader *vertexShader,
                                                 Diligent::IShader *pixelShader) {
    pipelineStateCreateInfo.pVS = vertexShader;
    pipelineStateCreateInfo.pPS = pixelShader;
}

void PipelineManager::PipelineObject::createPipelineState(Diligent::IRenderDevice *pDevice) {
    pDevice->CreateGraphicsPipelineState(pipelineStateCreateInfo, &state);
}

void PipelineManager::PipelineObject::createShaderBinding(bool initStaticResources) {
    state->CreateShaderResourceBinding(&binding, initStaticResources);
}

Diligent::IShaderResourceVariable *
PipelineManager::PipelineObject::getStaticVariableFromShader(
    Diligent::SHADER_TYPE shaderType, const char *name
) {
    return state->GetStaticVariableByName(shaderType, name);
}

Diligent::IShaderResourceVariable *
PipelineManager::PipelineObject::getVariableFromShader(
    Diligent::SHADER_TYPE shaderType, const char *name
) {
    return binding->GetVariableByName(shaderType, name);
}

Diligent::IShaderResourceVariable *
PipelineManager::PipelineObject::getStaticVariableFromVertexShader(const char *name) {
    return getStaticVariableFromShader(Diligent::SHADER_TYPE_PIXEL, name);
}

Diligent::IShaderResourceVariable *
PipelineManager::PipelineObject::getVariableFromVertexShader(const char *name) {
    return getVariableFromShader(Diligent::SHADER_TYPE_VERTEX, name);
}

Diligent::IShaderResourceVariable *
PipelineManager::PipelineObject::getStaticVariableFromPixelShader(const char *name) {
    return getStaticVariableFromShader(Diligent::SHADER_TYPE_PIXEL, name);
}

Diligent::IShaderResourceVariable *
PipelineManager::PipelineObject::getVariableFromPixelShader(const char *name) {
    return getVariableFromShader(Diligent::SHADER_TYPE_PIXEL, name);
}

const char *PipelineManager::PipelineObject::getName() {
    return pipelineStateCreateInfo.PSODesc.Name;
}

const Diligent::GraphicsPipelineStateCreateInfo &PipelineManager::PipelineObject::getPipelineStateCreateInfo() const {
    return pipelineStateCreateInfo;
}

bool PipelineManager::PipelineObject::keyMatches(const char *key) {
    return strlen(key) == keyLen && memcmp(getName(), key, keyLen) == 0;

}

void PipelineManager::PipelineObject::setDefaultResourceLayoutVariableType(
        Diligent::SHADER_RESOURCE_VARIABLE_TYPE type) {
    pipelineStateCreateInfo.PSODesc.ResourceLayout.DefaultVariableType = type;
}

PipelineManager::PipelineManager() {
    PSOs.table->page_size = 2;
}

PipelineManager::PipelineObject &PipelineManager::createPipeline(const char *key) {
    return PSOs.newObject(ObjectTypeNone, ObjectFlagNone, std::move(PipelineObject(key)))->resource.get<PipelineObject &>();
}

std::pair<PipelineManager::PipelineObject *, size_t> PipelineManager::findPipeline(const char *key) {
    auto iter = PSOs.table->getIterator();
    while (iter.hasNext()) {
        PipelineObject & po = iter.next()->resource.get<PipelineObject &>();
        if (po.keyMatches(key)) {
            return {&po, iter.getIndex()};
        }
    }
    return {nullptr, 0};
}

void PipelineManager::switchToPipeline(const char *key, Diligent::IDeviceContext *deviceContext) {
    std::pair<PipelineObject *, size_t> po = findPipeline(key);
    if (po.first != nullptr) {
        switchToPipeline(*po.first, deviceContext);
    }
}

void PipelineManager::switchToPipeline(PipelineManager::PipelineObject &pipelineObject,
                                       Diligent::IDeviceContext *deviceContext) {
    deviceContext->SetPipelineState(pipelineObject.getState());
}

void PipelineManager::commitShaderResourceBinding(const char *key,
                                                  Diligent::IDeviceContext *deviceContext,
                                                  Diligent::RESOURCE_STATE_TRANSITION_MODE resourceStateTransitionMode) {
    std::pair<PipelineObject *, size_t> po = findPipeline(key);
    if (po.first != nullptr) {
        commitShaderResourceBinding(*po.first, deviceContext, resourceStateTransitionMode);
    }
}

void PipelineManager::commitShaderResourceBinding(PipelineManager::PipelineObject &pipelineObject,
                                                  Diligent::IDeviceContext *deviceContext,
                                                  Diligent::RESOURCE_STATE_TRANSITION_MODE resourceStateTransitionMode) {
    auto & binding = pipelineObject.getBinding();
    if (binding) {
        deviceContext->CommitShaderResources(binding, resourceStateTransitionMode);
    }
}

void PipelineManager::switchToPipelineAndCommitShaderResourceBinding(
    const char *key, Diligent::IDeviceContext *deviceContext,
    Diligent::RESOURCE_STATE_TRANSITION_MODE resourceStateTransitionMode
) {
    std::pair<PipelineObject *, size_t> po = findPipeline(key);
    if (po.first != nullptr) {
        switchToPipelineAndCommitShaderResourceBinding(*po.first, deviceContext, resourceStateTransitionMode);
    }
}

void PipelineManager::switchToPipelineAndCommitShaderResourceBinding(
    PipelineManager::PipelineObject &pipelineObject, Diligent::IDeviceContext *deviceContext,
    Diligent::RESOURCE_STATE_TRANSITION_MODE resourceStateTransitionMode
) {
    deviceContext->SetPipelineState(pipelineObject.getState());
    commitShaderResourceBinding(pipelineObject, deviceContext, resourceStateTransitionMode);
}

void PipelineManager::destroyPipeline(const char *key) {
    std::pair<PipelineObject *, size_t> po = findPipeline(key);
    if (po.first != nullptr) {
        po.first->getState().Release();
        auto & binding = po.first->getBinding();
        if (binding) {
            binding.Release();
        }
        PSOs.table->DELETE(po.second);
    }
}
