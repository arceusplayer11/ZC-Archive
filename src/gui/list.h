#ifndef ZC_GUI_LIST_H
#define ZC_GUI_LIST_H

#include "widget.h"
#include "dialog_ref.h"
#include "list_data.h"
#include "../jwin.h"
#include <vector>

namespace GUI
{

class DialogRunner;

/* This and DropDownList are nearly identical. They should have a common
 * base class, or perhaps even be the same class. This is just a quick
 * and dirty copy-paste job.
 */
class List: public Widget
{
public:
	List();

	/* Sets the source of items to appear in the list. */
	void setListData(const ::GUI::ListData& newListData);

	/* Sets the value of the list item to select. If there is no item with
	 * a matching value, the closest possible will be selected. If there are
	 * multiple equally good matches, it is unspecified which will be selected.
	 */
	void setSelectedValue(int value);

	/* Selects the list item with the given index. */
	void setSelectedIndex(int index);

	/* Returns the value of the currently selected item. */
	int getSelectedValue() const;

	void setOnSelectFunc(std::function<void(int)> newOnSelect)
	{
		onSelectFunc = newOnSelect;
	}
	
	void setIsABC(bool abc);

	template<typename T>
	RequireMessage<T> onSelectionChanged(T m)
	{
		message = static_cast<int>(m);
	}

private:
	// A bit ugly because there was already a ListData struct in jwin
	::ListData jwinListData;
	const ::GUI::ListData* listData;
	int selectedIndex, selectedValue;
	DialogRef alDialog;
	int message;
	std::function<void(int)> onSelectFunc;
	bool isABC;

	/* If a value was set rather than an index, find an index to select. */
	void setIndex();
	void applyVisibility(bool visible) override;
	void realize(DialogRunner& runner) override;
	int onEvent(int event, MessageDispatcher& sendMessage) override;
};

}

#endif
