/****************************************************************************
 * Genesis Plus GX - GUI System
 *
 * Based on libwiigui from snes9xGC (Tantric 2009)
 * Adapted for Genesis Plus GX (2024)
 *
 * gui_window.cpp - Window container for GUI elements
 ***************************************************************************/

#include "gui.h"

GuiWindow::GuiWindow()
{
    width = 0;
    height = 0;
    focus = 0; // allow focus
}

GuiWindow::GuiWindow(int w, int h)
{
    width = w;
    height = h;
    focus = 0; // allow focus
}

GuiWindow::~GuiWindow()
{
    // Do not delete elements - ownership stays with caller
}

void GuiWindow::Append(GuiElement* e)
{
    if (e == NULL)
        return;
    
    Remove(e);
    _elements.push_back(e);
    e->SetParent(this);
}

void GuiWindow::Insert(GuiElement* e, u32 index)
{
    if (e == NULL || index > (_elements.size() - 1))
        return;
    
    Remove(e);
    _elements.insert(_elements.begin()+index, e);
    e->SetParent(this);
}

void GuiWindow::Remove(GuiElement* e)
{
    if (e == NULL)
        return;
    
    u32 elemSize = _elements.size();
    for (u32 i = 0; i < elemSize; ++i)
    {
        if(e == _elements.at(i))
        {
            _elements.erase(_elements.begin()+i);
            break;
        }
    }
}

void GuiWindow::RemoveAll()
{
    _elements.clear();
}

bool GuiWindow::Find(GuiElement* e)
{
    if (e == NULL)
        return false;
    
    u32 elemSize = _elements.size();
    for (u32 i = 0; i < elemSize; ++i)
        if(e == _elements.at(i))
            return true;
    return false;
}

GuiElement* GuiWindow::GetGuiElementAt(u32 index) const
{
    if (index >= _elements.size())
        return NULL;
    return _elements.at(index);
}

u32 GuiWindow::GetSize()
{
    return _elements.size();
}

void GuiWindow::SetVisible(bool v)
{
    visible = v;
}

void GuiWindow::Draw()
{
    if(_elements.size() == 0 || !this->IsVisible())
        return;
    
    u32 elemSize = _elements.size();
    for (u32 i = 0; i < elemSize; ++i)
    {
        _elements.at(i)->Draw();
    }
    
    this->UpdateEffects();
}

void GuiWindow::DrawTooltip()
{
    if(_elements.size() == 0 || !this->IsVisible())
        return;
    
    // TODO: Implement tooltip drawing when tooltip system is integrated
    // This will iterate through elements and draw their tooltips
    (void)0; // Placeholder
}

void GuiWindow::ResetState()
{
    if(state != STATE_DISABLED)
        state = STATE_DEFAULT;
    
    u32 elemSize = _elements.size();
    for (u32 i = 0; i < elemSize; ++i)
    {
        _elements.at(i)->ResetState();
    }
}

void GuiWindow::SetState(int s)
{
    state = s;
    
    u32 elemSize = _elements.size();
    for (u32 i = 0; i < elemSize; ++i)
    {
        _elements.at(i)->SetState(s);
    }
}

int GuiWindow::GetSelected()
{
    // Find which element is selected
    u32 elemSize = _elements.size();
    for (u32 i = 0; i < elemSize; ++i)
    {
        if(_elements.at(i)->GetState() == STATE_SELECTED)
            return i;
    }
    return -1;
}

void GuiWindow::SetFocus(int f)
{
    focus = f;
    
    if(f == 1)
    {
        // When gaining focus, try to select first selectable element
        u32 elemSize = _elements.size();
        for (u32 i = 0; i < elemSize; ++i)
        {
            if(_elements.at(i)->IsSelectable())
            {
                _elements.at(i)->SetState(STATE_SELECTED, -1);
                break;
            }
        }
    }
}

void GuiWindow::ChangeFocus(GuiElement * e)
{
    if(e == NULL)
        return;
    
    // Reset all elements to default, then select the specified one
    u32 elemSize = _elements.size();
    for (u32 i = 0; i < elemSize; ++i)
    {
        if(_elements.at(i) == e && _elements.at(i)->IsSelectable())
        {
            _elements.at(i)->SetState(STATE_SELECTED, -1);
        }
        else
        {
            _elements.at(i)->ResetState();
        }
    }
}

void GuiWindow::ToggleFocus(GuiTrigger * t)
{
    if(!t)
        return;
    
    // TODO: Implement focus cycling based on input
    // This will cycle through selectable elements based on button presses
    (void)0; // Placeholder
}

void GuiWindow::MoveSelectionHor(int dir)
{
    // TODO: Implement horizontal selection movement
    // This will move selection left/right among selectable elements
    (void)dir; // Suppress unused parameter warning
}

void GuiWindow::MoveSelectionVert(int dir)
{
    // TODO: Implement vertical selection movement
    // This will move selection up/down among selectable elements
    (void)dir; // Suppress unused parameter warning
}

void GuiWindow::ResetText()
{
    // Reset text for all child elements
    u32 elemSize = _elements.size();
    for (u32 i = 0; i < elemSize; ++i)
    {
        // TODO: Call ResetText on elements when GuiText integration is complete
        // For now, just iterate (no-op since base GuiElement doesn't have ResetText)
        (void)0;
    }
}

void GuiWindow::Update(GuiTrigger * t)
{
    if(state == STATE_DISABLED || !t || !this->IsVisible())
        return;
    
    // Update all visible and active elements
    u32 elemSize = _elements.size();
    for (u32 i = 0; i < elemSize; ++i)
    {
        GuiElement* element = _elements.at(i);
        
        if(element->IsVisible())
        {
            element->Update(t);
        }
    }
    
    this->UpdateEffects();
}
