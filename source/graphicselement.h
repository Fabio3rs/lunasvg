#ifndef GRAPHICSELEMENT_H
#define GRAPHICSELEMENT_H

#include "styledelement.h"

namespace lunasvg {

class GraphicsElement : public StyledElement
{
public:
    GraphicsElement(ElementID id);

    virtual void layout(LayoutContext* context, LayoutContainer* current) const;

    Transform transform() const;
    virtual Path path() const {
        return {};
    }
};

class TextElement : public GraphicsElement {
public:
    TextElement();

    void layout(LayoutContext* context, LayoutContainer* current) const;

    bool isText() const { return true; }

    Length x() const;
    Length y() const;
    Length rx() const;
    Length ry() const;
    Length width() const;
    Length height() const;

    Path path() const;

    std::unique_ptr<Node> clone() const;
};

} // namespace lunasvg

#endif // GRAPHICSELEMENT_H
