// mgimagesp.cpp
// Copyright (c) 2004-2013, Zhang Yungui
// License: LGPL, https://github.com/rhcad/touchvg

#include "mgimagesp.h"
#include "mgcomposite.h"
#include "mgshape_.h"
#include <string.h>

MG_IMPLEMENT_CREATE(MgImageShape)

MgImageShape::MgImageShape()
{
    _name[0] = 0;
}

MgImageShape::~MgImageShape()
{
}

void MgImageShape::setName(const char* name)
{
    int len = sizeof(_name) - 1;
#if defined(_MSC_VER) && _MSC_VER >= 1400 // VC8
    strncpy_s(_name, sizeof(_name), name, len);
#else
    strncpy(_name, name, len);
#endif
    _name[len] = 0;
    
    if (strstr(_name, "%d.")) {
        setFlag(kMgHideContent, true);
        setFlag(kMgNoClone, true);
    }
}

bool MgImageShape::_draw(int, GiGraphics& gs, const GiContext& ctx, int) const
{
    Box2d rect(getRect() * gs.xf().modelToDisplay());
    Vector2d vec((_points[1] - _points[0]) * gs.xf().modelToWorld());
    bool ret = false;
    
    if (isVisible()) {
        ret = (gs.rawImage(_name, rect.center().x, rect.center().y,
                           rect.width(), rect.height(), vec.angle2())
               || drawBox(gs, ctx));
    }
    
    return ret;
}

bool MgImageShape::drawBox(GiGraphics& gs, const GiContext& ctx) const
{
    GiContext tmpctx(ctx);
    tmpctx.setNoFillColor();
    tmpctx.setLineStyle(GiContext::kSolidLine);
    
    GiContext ctxline(tmpctx);
    ctxline.setLineWidth(0, false);
    
    return (gs.drawPolygon(&tmpctx, 4, _points)
            && gs.drawLine(&ctxline, _points[0], _points[2])
            && gs.drawLine(&ctxline, _points[1], _points[3]));
}

void MgImageShape::_copy(const MgImageShape& src)
{
#if defined(_MSC_VER) && _MSC_VER >= 1400 // VC8
    strcpy_s(_name, sizeof(_name), src._name);
#else
    strcpy(_name, src._name);
#endif
    __super::_copy(src);
}

bool MgImageShape::_equals(const MgImageShape& src) const
{
    return strcmp(_name, src._name) == 0 && __super::_equals(src);
}

void MgImageShape::_clear()
{
    _name[0] = 0;
    __super::_clear();
}

bool MgImageShape::_save(MgStorage* s) const
{
    s->writeString("name", _name);
    s->writeFloat("imageWidth", _size.x);
    s->writeFloat("imageHeight", _size.y);
    return __super::_save(s);
}

bool MgImageShape::_load(MgShapeFactory* factory, MgStorage* s)
{
    int len = sizeof(_name) - 1;
    len = s->readString("name", _name, len);
    _name[len] = 0;
    
    _size.set(s->readFloat("imageWidth", 0), s->readFloat("imageHeight", 0));
    if (_size.x < 1 || _size.y < 1) {
        s->setError("Invalid image size");
        return false;
    }
    
    return __super::_load(factory, s);
}

const MgShape* MgImageShape::findShapeByImageID(const MgShapes* shapes, const char* name)
{
    MgShapeIterator it(shapes);
    const MgShape* ret = NULL;
    
    while (const MgShape* sp = it.getNext()) {
        if (sp->shapec()->isKindOf(MgImageShape::Type())) {
            const MgImageShape *image = (const MgImageShape*)sp->shapec();
            if (strcmp(name, image->getName()) == 0) {
                ret = sp;
                break;
            }
        } else if (sp->shapec()->isKindOf(MgComposite::Type())) {
            const MgComposite *composite = (const MgComposite *)sp->shapec();
            ret = findShapeByImageID(composite->shapes(), name);
            if (ret) {
                break;
            }
        }
    }
    
    return ret;
}
