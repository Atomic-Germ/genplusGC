/****************************************************************************
 * Genesis Plus GX - GUI System
 * gui_element.cpp
 *
 * GuiElement - Base class for all GUI elements
 ***************************************************************************/

#include "gui.h"

GuiElement::GuiElement()
{
    xoffset = 0;
    yoffset = 0;
    xmin = 0;
    xmax = 0;
    ymin = 0;
    ymax = 0;
    width = 0;
    height = 0;
    alpha = 255;
    xscale = 1;
    yscale = 1;
    state = STATE_DEFAULT;
    stateChan = -1;
    trigger[0] = NULL;
    trigger[1] = NULL;
    trigger[2] = NULL;
    trigger[3] = NULL;
    trigger[4] = NULL;
    parentElement = NULL;
    rumble = true;
    selectable = false;
    clickable = false;
    holdable = false;
    visible = true;
    focus = -1;
    updateCB = NULL;
    yoffsetDyn = 0;
    xoffsetDyn = 0;
    alphaDyn = -1;
    scaleDyn = 1;
    effects = 0;
    effectAmount = 0;
    effectTarget = 0;
    effectsOver = 0;
    effectAmountOver = 0;
    effectTargetOver = 0;
    alignmentVert = ALIGN_TOP;
    alignmentHor = ALIGN_LEFT;
}

GuiElement::~GuiElement()
{
}

void GuiElement::SetParent(GuiElement * e)
{
    parentElement = e;
}

GuiElement * GuiElement::GetParent()
{
    return parentElement;
}

int GuiElement::GetLeft()
{
    int x = 0;
    int pWidth = 0;
    int pLeft = 0;

    if(parentElement)
    {
        pWidth = parentElement->GetWidth();
        pLeft = parentElement->GetLeft();
    }

    if(effects & EFFECT_SLIDE_IN || effects & EFFECT_SLIDE_OUT)
        pLeft += xoffsetDyn;

    switch(alignmentHor)
    {
        case ALIGN_LEFT:
            x = pLeft;
            break;
        case ALIGN_CENTRE:
            x = pLeft + (pWidth/2) - (width/2);
            break;
        case ALIGN_RIGHT:
            x = pLeft + pWidth - width;
            break;
    }
    return x + xoffset;
}

int GuiElement::GetTop()
{
    int y = 0;
    int pHeight = 0;
    int pTop = 0;

    if(parentElement)
    {
        pHeight = parentElement->GetHeight();
        pTop = parentElement->GetTop();
    }

    if(effects & EFFECT_SLIDE_IN || effects & EFFECT_SLIDE_OUT)
        pTop += yoffsetDyn;

    switch(alignmentVert)
    {
        case ALIGN_TOP:
            y = pTop;
            break;
        case ALIGN_MIDDLE:
            y = pTop + (pHeight/2) - (height/2);
            break;
        case ALIGN_BOTTOM:
            y = pTop + pHeight - height;
            break;
    }
    return y + yoffset;
}

int GuiElement::GetWidth()
{
    return width;
}

int GuiElement::GetHeight()
{
    return height;
}

void GuiElement::SetSize(int w, int h)
{
    width = w;
    height = h;
}

bool GuiElement::IsVisible()
{
    return visible;
}

bool GuiElement::IsSelectable()
{
    return selectable;
}

bool GuiElement::IsClickable()
{
    return clickable;
}

bool GuiElement::IsHoldable()
{
    return holdable;
}

void GuiElement::SetSelectable(bool s)
{
    selectable = s;
}

void GuiElement::SetClickable(bool c)
{
    clickable = c;
}

void GuiElement::SetHoldable(bool h)
{
    holdable = h;
}

int GuiElement::GetState()
{
    return state;
}

int GuiElement::GetStateChan()
{
    return stateChan;
}

void GuiElement::SetAlpha(int a)
{
    alpha = a;
}

int GuiElement::GetAlpha()
{
    int a;

    if(alphaDyn >= 0)
        a = alphaDyn;
    else
        a = alpha;

    if(parentElement)
        a = (a * parentElement->GetAlpha()) / 255;

    return a;
}

void GuiElement::SetScale(float s)
{
    xscale = s;
    yscale = s;
}

float GuiElement::GetScale()
{
    float s = scaleDyn * xscale;

    if(parentElement)
        s *= parentElement->GetScale();

    return s;
}

void GuiElement::SetTrigger(GuiTrigger * t)
{
    trigger[0] = t;
}

void GuiElement::SetTrigger(u8 i, GuiTrigger * t)
{
    if(i < 5)
        trigger[i] = t;
}

void GuiElement::SetPosition(int x, int y)
{
    xoffset = x;
    yoffset = y;
}

void GuiElement::SetAlignment(int hor, int vert)
{
    alignmentHor = hor;
    alignmentVert = vert;
}

void GuiElement::SetEffect(int e, int a, int t)
{
    effects = e;
    effectAmount = a;
    effectTarget = t;
}

void GuiElement::SetEffectOnOver(int e, int a, int t)
{
    effectsOver = e;
    effectAmountOver = a;
    effectTargetOver = t;
}

void GuiElement::SetEffectGrow()
{
    SetEffectOnOver(EFFECT_SCALE, 4, 110);
}

bool GuiElement::IsInside(int x, int y)
{
    if(x > this->GetLeft() && x < (this->GetLeft()+width)
    && y > this->GetTop() && y < (this->GetTop()+height))
        return true;
    return false;
}

void GuiElement::SetState(int s, int c)
{
    state = s;
    stateChan = c;
}

void GuiElement::ResetState()
{
    state = STATE_DEFAULT;
    stateChan = -1;
}

void GuiElement::SetVisible(bool v)
{
    visible = v;
}

void GuiElement::SetFocus(int f)
{
    focus = f;
}

int GuiElement::IsFocused()
{
    return focus;
}

void GuiElement::UpdateEffects()
{
    if(effects & EFFECT_SLIDE_IN)
    {
        if(effects & EFFECT_SLIDE_LEFT)
        {
            xoffsetDyn += effectAmount;
            if(xoffsetDyn >= 0)
            {
                xoffsetDyn = 0;
                effects = 0;
            }
        }
        else if(effects & EFFECT_SLIDE_RIGHT)
        {
            xoffsetDyn -= effectAmount;
            if(xoffsetDyn <= 0)
            {
                xoffsetDyn = 0;
                effects = 0;
            }
        }
        else if(effects & EFFECT_SLIDE_TOP)
        {
            yoffsetDyn += effectAmount;
            if(yoffsetDyn >= 0)
            {
                yoffsetDyn = 0;
                effects = 0;
            }
        }
        else if(effects & EFFECT_SLIDE_BOTTOM)
        {
            yoffsetDyn -= effectAmount;
            if(yoffsetDyn <= 0)
            {
                yoffsetDyn = 0;
                effects = 0;
            }
        }
    }
    else if(effects & EFFECT_SLIDE_OUT)
    {
        if(effects & EFFECT_SLIDE_LEFT)
        {
            xoffsetDyn -= effectAmount;
            if(xoffsetDyn <= -width)
                effects = 0;
        }
        else if(effects & EFFECT_SLIDE_RIGHT)
        {
            xoffsetDyn += effectAmount;
            if(xoffsetDyn >= width)
                effects = 0;
        }
        else if(effects & EFFECT_SLIDE_TOP)
        {
            yoffsetDyn -= effectAmount;
            if(yoffsetDyn <= -height)
                effects = 0;
        }
        else if(effects & EFFECT_SLIDE_BOTTOM)
        {
            yoffsetDyn += effectAmount;
            if(yoffsetDyn >= height)
                effects = 0;
        }
    }
    else if(effects & EFFECT_FADE)
    {
        alphaDyn += effectAmount;

        if(effectAmount < 0 && alphaDyn <= effectTarget)
        {
            alphaDyn = effectTarget;
            effects = 0;
        }
        else if(effectAmount > 0 && alphaDyn >= effectTarget)
        {
            alphaDyn = effectTarget;
            effects = 0;
        }
    }
    else if(effects & EFFECT_SCALE)
    {
        scaleDyn += ((float)effectAmount/100.0f);

        if((effectAmount < 0 && scaleDyn <= ((float)effectTarget/100.0f))
        || (effectAmount > 0 && scaleDyn >= ((float)effectTarget/100.0f)))
        {
            scaleDyn = (float)effectTarget/100.0f;
            effects = 0;
        }
    }
}

void GuiElement::Update(GuiTrigger * t)
{
    (void)t;  // Suppress unused parameter warning
    if(updateCB)
        updateCB(this);
}

void GuiElement::Draw()
{
}
