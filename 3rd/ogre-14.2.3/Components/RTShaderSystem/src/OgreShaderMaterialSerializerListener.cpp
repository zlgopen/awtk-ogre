/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org

Copyright (c) 2000-2014 Torus Knot Software Ltd
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "OgreShaderPrecompiledHeaders.h"

namespace Ogre {
namespace RTShader {

//-----------------------------------------------------------------------------
SGMaterialSerializerListener::SGMaterialSerializerListener()
{
    mSourceMaterial = NULL;
}

//-----------------------------------------------------------------------------
void SGMaterialSerializerListener::materialEventRaised(MaterialSerializer* ser,
                                                        MaterialSerializer::SerializeEvent event,
                                                        bool& skip, const Material* mat)
{
    if (event == MaterialSerializer::MSE_PRE_WRITE)
    {
        MaterialPtr matPtr = MaterialManager::getSingleton().getByName(mat->getName());
        mSourceMaterial = matPtr.get();
        mSGPassList = ShaderGenerator::getSingleton().createSGPassList(mSourceMaterial);
    }

    if (event == MaterialSerializer::MSE_POST_WRITE)
    {
        mSourceMaterial = NULL;
        mSGPassList.clear();
    }
}

//-----------------------------------------------------------------------------
void SGMaterialSerializerListener::techniqueEventRaised(MaterialSerializer* ser,
                                          MaterialSerializer::SerializeEvent event,
                                          bool& skip, const Technique* tech)
{
    // Pre technique write event.
    if (event == MaterialSerializer::MSE_PRE_WRITE)
    {
        const Any& techUserData = tech->getUserObjectBindings().getUserAny(ShaderGenerator::SGTechnique::UserKey);

        // Skip writing this technique since it was generated by the Shader Generator.
        if (techUserData.has_value())
        {
            skip = true;
            return;
        }
    }
}

//-----------------------------------------------------------------------------
void SGMaterialSerializerListener::serializePassAttributes(MaterialSerializer* ser, ShaderGenerator::SGPass* passEntry)
{
    // Grab the custom render state this pass uses.
    RenderState* customRenderState = passEntry->getCustomRenderState();

    if (!customRenderState)
        return;

     // Write section header and begin it.
    ser->writeAttribute(3, "rtshader_system");
    ser->beginSection(3);

    // Write each of the sub-render states that composing the final render state.
    for (SubRenderState* curSubRenderState : customRenderState->getSubRenderStates())
    {
        if (SubRenderStateFactory* curFactory =
                ShaderGenerator::getSingleton().getSubRenderStateFactory(curSubRenderState->getType()))
        {
            curFactory->writeInstance(ser, curSubRenderState, passEntry->getSrcPass(), passEntry->getDstPass());
        }
    }

    // Write section end.
    ser->endSection(3);
}

//-----------------------------------------------------------------------------
void SGMaterialSerializerListener::serializeTextureUnitStateAttributes(MaterialSerializer* ser,
                                                                       ShaderGenerator::SGPass* passEntry,
                                                                       const TextureUnitState* srcTextureUnit)
{
    // Grab the custom render state this pass uses.
    RenderState* customRenderState = passEntry->getCustomRenderState();

    if (!customRenderState)
        return;

    // Write section header and begin it.
    ser->writeAttribute(4, "rtshader_system");
    ser->beginSection(4);

    //retrive the destintion texture unit state
    TextureUnitState* dstTextureUnit = NULL;
    unsigned short texIndex = srcTextureUnit->getParent()->getTextureUnitStateIndex(srcTextureUnit);
    if (texIndex < passEntry->getDstPass()->getNumTextureUnitStates())
    {
        dstTextureUnit = passEntry->getDstPass()->getTextureUnitState(texIndex);
    }

    // Write each of the sub-render states that composing the final render state.
    for (SubRenderState* curSubRenderState : customRenderState->getSubRenderStates())
    {
        if (SubRenderStateFactory* curFactory =
                ShaderGenerator::getSingleton().getSubRenderStateFactory(curSubRenderState->getType()))
            curFactory->writeInstance(ser, curSubRenderState, srcTextureUnit, dstTextureUnit);
    }

    // Write section end.
    ser->endSection(4);
}

//-----------------------------------------------------------------------------
void SGMaterialSerializerListener::passEventRaised(MaterialSerializer* ser,
                                                   MaterialSerializer::SerializeEvent event,
                                                   bool& skip, const Pass* pass)
{
    // End of pass writing event.
    if (event == MaterialSerializer::MSE_WRITE_END)
    {
        // Case this pass use as source pass for shader generated pass.
        if (auto passEntry = getShaderGeneratedPass(pass))
            serializePassAttributes(ser, passEntry);
    }
}

//-----------------------------------------------------------------------------
void SGMaterialSerializerListener::textureUnitStateEventRaised(MaterialSerializer* ser,
                                            MaterialSerializer::SerializeEvent event,
                                            bool& skip, const TextureUnitState* textureUnit)
{
    // End of pass writing event.
    if (event == MaterialSerializer::MSE_WRITE_END)
    {
        // Case this pass use as source pass for shader generated pass.
        if (auto passEntry = getShaderGeneratedPass(textureUnit->getParent()))
            serializeTextureUnitStateAttributes(ser, passEntry, textureUnit);
    }
}

//-----------------------------------------------------------------------------
ShaderGenerator::SGPass* SGMaterialSerializerListener::getShaderGeneratedPass(const Pass* srcPass)
{
    for (auto *s : mSGPassList)
    {
        if (s->getSrcPass() == srcPass)
            return s;
    }

    return NULL;
}

}
}