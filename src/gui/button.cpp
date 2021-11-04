#include "button.h"
#include "common.h"
#include "dialog.h"
#include "dialog_runner.h"
#include "../jwin.h"
#include "../zquest.h"
#include <algorithm>
#include <utility>

namespace GUI
{

Button::Button(): text(), message(-1)
{
	setPreferredHeight(2.5_em);
}

void Button::setText(std::string newText)
{
	Size targetWidth = Size::em(newText.size());
	setPreferredWidth(std::max(targetWidth, 90_lpx));
	text = std::move(newText);
}

void Button::setOnPress(std::function<void()> newOnPress)
{
	onPress = std::move(newOnPress);
}

void Button::applyVisibility(bool visible)
{
	Widget::applyVisibility(visible);
	if(alDialog) alDialog.applyVisibility(visible);
}

void Button::applyFont(FONT* newFont)
{
	if(alDialog)
	{
		alDialog->dp2 = newFont;
	}
	Widget::applyFont(newFont);
}

void Button::realize(DialogRunner& runner)
{
	Widget::realize(runner);
	alDialog = runner.push(shared_from_this(), DIALOG {
		newGUIProc<jwin_button_proc>,
		x, y, getWidth(), getHeight(),
		fgColor, bgColor,
		getAccelKey(text),
		getFlags(),
		0, 0, // d1, d2
		text.data(), widgFont, nullptr // dp, dp2, dp3
	});
}

int32_t Button::onEvent(int32_t event, MessageDispatcher& sendMessage)
{
	assert(event == geCLICK);
	// jwin_button_proc doesn't seem to allow for a non-toggle button...
	alDialog->flags &= ~D_SELECTED;
	
	if(onPress)
		onPress();
	if(message >= 0)
		sendMessage(message, MessageArg::none);
	return -1;
}

}