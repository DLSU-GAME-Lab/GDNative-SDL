#include "ScrollInput.h"

ScrollInput::ScrollInput(ScrollView* pScrollView):AGeneralInput("ScrollInput")
{
	this->pScrollView = pScrollView;
}

ScrollInput::~ScrollInput()
{
}

void ScrollInput::perform()
{
	if (!eEvent || eEvent->type != SDL_EVENT_MOUSE_WHEEL || !pScrollView->getEnabled())
		return;

	const int scrollSpeed = 40;
	int offset = pScrollView->getScrollOffset(); // Add getter
	offset -= eEvent->wheel.y * scrollSpeed;
	offset = std::max(0, std::min(offset, pScrollView->getContentHeight() - pScrollView->getViewHeight()));
	pScrollView->setScrollOffset(offset); // Add setter

}
