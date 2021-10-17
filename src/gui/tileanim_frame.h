#ifndef ZC_GUI_TILEANIM_FRAME_H
#define ZC_GUI_TILEANIM_FRAME_H

#include "widget.h"
#include "dialog_ref.h"

int tile_anim_proc(int32_t msg,DIALOG *d,int32_t c);

namespace GUI
{

class TileFrame: public Widget
{
public:
	TileFrame();

	void setTile(int32_t value);
	void setCSet(int32_t value);
	void setFrames(int32_t value);
	void setSpeed(int32_t value);
	void setDelay(int32_t value);
	
	void resetAnim();
	
private:
	enum { tfr_tile, tfr_cset, tfr_frames, tfr_speed, tfr_delay, tfr_aclk, tfr_aframe, tfr_MAX };
	int32_t data[tfr_MAX];
	DialogRef alDialog;
	
	void applyVisibility(bool visible) override;
	void realize(DialogRunner& runner) override;
	friend int ::tile_anim_proc(int32_t,DIALOG *,int32_t);
};

}

#endif