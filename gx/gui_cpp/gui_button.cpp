/****************************************************************************
 * Genesis Plus GX - GUI Button
 * 
 * Interactive button widget combining images, text, and sounds
 * Based on libwiigui button implementation
 ***************************************************************************/

#include "gui.h"

/**
 * Constructor for the GuiButton class.
 */
GuiButton::GuiButton(int w, int h)
{
    width = w;
    height = h;
    
    // Initialize all pointers to NULL
    image = NULL;
    imageOver = NULL;
    imageHold = NULL;
    imageClick = NULL;
    icon = NULL;
    iconOver = NULL;
    label = NULL;
    labelOver = NULL;
    soundOver = NULL;
    soundHold = NULL;
    soundClick = NULL;
    
    // Make buttons selectable and clickable by default
    selectable = true;
    clickable = true;
}

/**
 * Destructor for the GuiButton class.
 * Note: Button does NOT own child elements (images, text, sounds)
 * Memory management is the caller's responsibility
 */
GuiButton::~GuiButton()
{
    // Do not delete child elements - they are managed by the caller
}

/**
 * Set the button's default image
 */
void GuiButton::SetImage(GuiImage* img)
{
    image = img;
    if(img)
        img->SetParent(this);
}

/**
 * Set the button's image for STATE_SELECTED (hover)
 */
void GuiButton::SetImageOver(GuiImage* img)
{
    imageOver = img;
    if(img)
        img->SetParent(this);
}

/**
 * Set the button's image for STATE_HELD
 */
void GuiButton::SetImageHold(GuiImage* img)
{
    imageHold = img;
    if(img)
        img->SetParent(this);
}

/**
 * Set the button's image for STATE_CLICKED
 */
void GuiButton::SetImageClick(GuiImage* img)
{
    imageClick = img;
    if(img)
        img->SetParent(this);
}

/**
 * Set the button's icon (drawn over the image)
 */
void GuiButton::SetIcon(GuiImage* img)
{
    icon = img;
    if(img)
        img->SetParent(this);
}

/**
 * Set the button's icon for STATE_SELECTED (hover)
 */
void GuiButton::SetIconOver(GuiImage* img)
{
    iconOver = img;
    if(img)
        img->SetParent(this);
}

/**
 * Set the button's label text
 */
void GuiButton::SetLabel(GuiText* txt)
{
    label = txt;
    if(txt)
        txt->SetParent(this);
}

/**
 * Set the button's label text for STATE_SELECTED (hover)
 */
void GuiButton::SetLabelOver(GuiText* txt)
{
    labelOver = txt;
    if(txt)
        txt->SetParent(this);
}

/**
 * Set the sound to play when entering STATE_SELECTED (hover)
 */
void GuiButton::SetSoundOver(GuiSound * snd)
{
    soundOver = snd;
}

/**
 * Set the sound to play when entering STATE_HELD
 */
void GuiButton::SetSoundHold(GuiSound * snd)
{
    soundHold = snd;
}

/**
 * Set the sound to play when entering STATE_CLICKED
 */
void GuiButton::SetSoundClick(GuiSound * snd)
{
    soundClick = snd;
}

/**
 * Draw the button on screen
 * Selects appropriate image/icon/label based on state
 */
void GuiButton::Draw()
{
    if(!this->IsVisible())
        return;
    
    // Determine which image to draw based on state
    GuiImage * imageToDraw = image;
    GuiImage * iconToDraw = icon;
    GuiText * labelToDraw = label;
    
    // State-based image selection
    if(state == STATE_SELECTED)
    {
        if(imageOver)
            imageToDraw = imageOver;
        if(iconOver)
            iconToDraw = iconOver;
        if(labelOver)
            labelToDraw = labelOver;
    }
    else if(state == STATE_HELD)
    {
        if(imageHold)
            imageToDraw = imageHold;
        else if(imageOver)
            imageToDraw = imageOver;
        
        // For held state, use over versions if available
        if(iconOver)
            iconToDraw = iconOver;
        if(labelOver)
            labelToDraw = labelOver;
    }
    else if(state == STATE_CLICKED)
    {
        if(imageClick)
            imageToDraw = imageClick;
        else if(imageHold)
            imageToDraw = imageHold;
        else if(imageOver)
            imageToDraw = imageOver;
        
        // For clicked state, use over versions if available
        if(iconOver)
            iconToDraw = iconOver;
        if(labelOver)
            labelToDraw = labelOver;
    }
    
    // Draw in order: image, icon, label (text on top)
    if(imageToDraw)
        imageToDraw->Draw();
    
    if(iconToDraw)
        iconToDraw->Draw();
    
    if(labelToDraw)
        labelToDraw->Draw();
    
    this->UpdateEffects();
}

/**
 * Update button state based on trigger input
 * Handles hover, press, hold, and click detection
 * 
 * Note: This is a simplified implementation. Full implementation would:
 * - Check actual controller input (PAD/WPAD)
 * - Handle cursor position for hover detection
 * - Manage button press/hold/release states
 * For now, we focus on the state machine logic and structure.
 */
void GuiButton::Update(GuiTrigger * t)
{
    if(state == STATE_CLICKED || state == STATE_DISABLED || !t)
        return;
    
    // Check if parent is disabled
    if(parentElement && parentElement->GetState() == STATE_DISABLED)
        return;
    
    // Update effects
    GuiElement::Update(t);
    
    // TODO: In full implementation, check cursor position and trigger input
    // For now, we implement the state transition logic structure
    
    // Placeholder for cursor check - in full implementation would check actual position
    // bool isInside = this->IsInside(cursor_x, cursor_y);
    
    // Example state transition logic (to be filled in during integration):
    // 
    // if(isInside && state == STATE_DEFAULT)
    // {
    //     this->SetState(STATE_SELECTED, t->chan);
    //     if(soundOver) soundOver->Play();
    // }
    // else if(!isInside && state == STATE_SELECTED)
    // {
    //     this->ResetState();
    // }
    //
    // Check for button press across all configured triggers
    // for(int i = 0; i < 6; i++)
    // {
    //     if(!trigger[i]) continue;
    //     
    //     if(trigger matches input)
    //     {
    //         if(state == STATE_SELECTED)
    //         {
    //             this->SetState(STATE_CLICKED, t->chan);
    //             if(soundClick) soundClick->Play();
    //         }
    //         else if(trigger[i]->type == TRIGGER_BUTTON_ONLY)
    //         {
    //             this->SetState(STATE_CLICKED, t->chan);
    //             if(soundClick) soundClick->Play();
    //         }
    //     }
    // }
    //
    // Handle holdable buttons
    // if(this->IsHoldable() && trigger[i]->type == TRIGGER_HELD)
    // {
    //     if(button_held)
    //     {
    //         if(state == STATE_CLICKED)
    //         {
    //             this->SetState(STATE_HELD, t->chan);
    //             if(soundHold) soundHold->Play();
    //         }
    //     }
    //     else if(state == STATE_HELD)
    //     {
    //         this->ResetState();
    //     }
    // }
    
    // Call update callback if defined
    if(updateCB)
        updateCB(this);
}
