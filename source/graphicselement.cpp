#include "graphicselement.h"
#include "element.h"
#include "layoutcontext.h"
#include "parser.h"
#include <iostream>

namespace lunasvg {

GraphicsElement::GraphicsElement(ElementID id) : StyledElement(id) {}

void GraphicsElement::layout(LayoutContext *context,
                             LayoutContainer *current) const {
    if (isDisplayNone())
        return;

    if (this->children.empty()) {
        return;
    }

    auto textNode = dynamic_cast<const TextNode *>(this->children.back().get());

    if (!textNode) {
        return;
    }

    auto shape = makeUnique<LayoutText>();
    shape->fillData = context->fillData(this);
    shape->strokeData = context->strokeData(this);

    shape->text = textNode->text;

    current->addChild(std::move(shape));
}

Transform GraphicsElement::transform() const {
    auto &value = get(PropertyID::Transform);
    return Parser::parseTransform(value);
}

TextElement::TextElement() : GraphicsElement(ElementID::Rect) {}

void TextElement::layout(LayoutContext *context,
                         LayoutContainer *current) const {
    if (isDisplayNone())
        return;

    if (this->children.empty()) {
        return;
    }

    auto textNode = dynamic_cast<const TextNode *>(this->children.back().get());

    if (!textNode) {
        return;
    }

    std::cout << std::hex << fill().color().value() << std::endl;
    std::cout << std::hex << stroke().color().value() << std::endl;

    std::cout << std::hex << color().value() << std::endl;
    std::cout << std::hex << solid_color().value() << std::endl;

    auto shape = makeUnique<LayoutText>();

    shape->text = textNode->text;
    LengthContext lengthContext(this);
    shape->x = lengthContext.valueForLength(x(), LengthMode::Width);
    shape->y = lengthContext.valueForLength(y(), LengthMode::Height);
    shape->fillData = context->fillData(this);
    shape->strokeData = context->strokeData(this);

    current->addChild(std::move(shape));
}

Length TextElement::x() const {
    auto &value = get(PropertyID::X);
    return Parser::parseLength(value, AllowNegativeLengths, Length::Zero);
}

Length TextElement::y() const {
    auto &value = get(PropertyID::Y);
    return Parser::parseLength(value, AllowNegativeLengths, Length::Zero);
}

Length TextElement::rx() const {
    auto &value = get(PropertyID::Rx);
    return Parser::parseLength(value, ForbidNegativeLengths, Length::Unknown);
}

Length TextElement::ry() const {
    auto &value = get(PropertyID::Ry);
    return Parser::parseLength(value, ForbidNegativeLengths, Length::Unknown);
}

Length TextElement::width() const {
    auto &value = get(PropertyID::Width);
    return Parser::parseLength(value, ForbidNegativeLengths, Length::Zero);
}

Length TextElement::height() const {
    auto &value = get(PropertyID::Height);
    return Parser::parseLength(value, ForbidNegativeLengths, Length::Zero);
}

Path TextElement::path() const {
    auto w = this->width();
    auto h = this->height();
    if (w.isZero() || h.isZero())
        return Path{};

    LengthContext lengthContext(this);
    auto _x = lengthContext.valueForLength(x(), LengthMode::Width);
    auto _y = lengthContext.valueForLength(y(), LengthMode::Height);
    auto _w = lengthContext.valueForLength(w, LengthMode::Width);
    auto _h = lengthContext.valueForLength(h, LengthMode::Height);

    auto rx = this->rx();
    auto ry = this->ry();

    auto _rx = lengthContext.valueForLength(rx, LengthMode::Width);
    auto _ry = lengthContext.valueForLength(ry, LengthMode::Height);

    if (!rx.isValid())
        _rx = _ry;
    if (!ry.isValid())
        _ry = _rx;

    Path path;
    path.rect(_x, _y, _w, _h, _rx, _ry);
    return path;
}

std::unique_ptr<Node> TextElement::clone() const {
    return cloneElement<TextElement>();
}
} // namespace lunasvg
