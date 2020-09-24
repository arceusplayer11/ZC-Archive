/* This is such a mess. We have Music_Emu*, GMEFILE*, and ZCMUSIC* all somehow intermixed ust to play a chiptune.
I'm not yet certain if we can load a chiptune in this anner than direct zc_music to play it out of the zcchiptunes struct's *data element.

perhaps *data needs to be of the type AUDIOSTREAM *data?! Then we load it when playing into the correct structure?

typedef struct GMEFILE : public ZCMUSICBASE
{
    AUDIOSTREAM *stream;
    class Music_Emu* emu;
    int samples;
} GMEFILE;

typedef struct ZCMUSICBASE
{
    int type;                                               // uses ZCMF defines
    int playing;                                            // -1 = paused, 0 = stopped, 1 = playing
    int position;                                           // Only needed to sync Triforce jingle
    char filename[256];
    int track;
} ZCMUSIC;



*/


/*******************************/
/**********  onMidis  **********/
/*******************************/


//! needs TRACK ID, and convesions to GMEFILE* and DUH*
static DIALOG editchiptune_dlg[] =
{
    /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)     (bg)    (key)    (flags)     (d1)           (d2)     (dp) */
    { jwin_win_proc,     24,   20,   273,  189,  vc(14),  vc(1),  0,       D_EXIT,          0,             0, (void *) "Chiptiune Specs", NULL, NULL },
    { d_timer_proc,         0,    0,     0,    0,    0,       0,       0,       0,          0,          0,         NULL, NULL, NULL },
    // 2
    { d_dummy_proc,       56,   94-16,   48,   8,    vc(14),  vc(1),  0,       0,          0,             0, (void *) "File:", NULL, NULL },
    { d_dummy_proc,       104,  94-16,   48,   8,    vc(11),  vc(1),  0,       0,          0,             0,       NULL, NULL, NULL },
	//4
    { jwin_text_proc,       56,   114,  48,   8,    vc(14),  vc(1),  0,       0,          0,             0, (void *) "Name:", NULL, NULL },
    //needs to be a text proc:
   // { jwin_edit_proc,       104,  114-4,  160,  16,   vc(12),  vc(1),  0,       0,          35,            0,       NULL, NULL, NULL },
    { jwin_text_proc,       104,  114-4,  160,  16,   vc(12),  vc(1),  0,       0,          35,            0,       NULL, NULL, NULL },
    { d_dummy_proc,       56,   124-4+12,  56,   8,    vc(14),  vc(1),  0,       0,          0,             0, (void *) "Volume:", NULL, NULL },
    { d_dummy_proc,       120,  124-4+12-4,  32,   16,   vc(12),  vc(1),  0,       0,          3,             0,       NULL, NULL, NULL },
    // 8
    { d_dummy_proc,      176,  124+12-4,  80+1,   8+1,    vc(14),  vc(1),  0,       0,          1,             0, (void *) "Loop", NULL, NULL },
    // 9, load
    { jwin_button_proc,     50,   72-24,   57,   21,   vc(14),  vc(1),  'l',     D_EXIT,     0,             0, (void *) "&Load", NULL, NULL },
    //10, stop
    { jwin_button_proc,     116,  72-24,   33,   21,   vc(14),  vc(1),  0,       D_EXIT,     0,             0, (void *) "\x8D", NULL, NULL },
    //11, play
    { jwin_button_proc,     156,  72-24,   33,   21,   vc(14),  vc(1),  0,       D_EXIT,     0,             0, (void *) "\x8B", NULL, NULL },
    { d_dummy_proc,     196,  72-24,   33,   21,   vc(14),  vc(1),  0,       D_EXIT,     0,             0, (void *) "\x8B\x8B", NULL, NULL },
    { d_dummy_proc,     236,  72-24,   33,   21,   vc(14),  vc(1),  0,       D_EXIT,     0,             0, (void *) "\x8B\x8B\x8B", NULL, NULL },
    // 14
    { d_dummy_proc,       56,   134+4+12,  48,   8,    vc(14),  vc(1),  0,       0,          0,             0, (void *) "Start:", NULL, NULL },
    { d_dummy_proc,       112,  134+12,  32,   16,   vc(12),  vc(1),  0,       0,          5,             0,       NULL, NULL, NULL },
    { d_dummy_proc,       176,  134+12+4,  56,   8,    vc(14),  vc(1),  0,       0,          0,             0, (void *) "Loop Start:", NULL, NULL },
    { d_dummy_proc,       240,  134+12,  40,   16,   vc(12),  vc(1),  0,       0,          5,             0,       NULL, NULL, NULL },
    { d_dummy_proc,       176,  144+12+12,  48,   8,    vc(14),  vc(1),  0,       0,          0,             0, (void *) "Loop End:", NULL, NULL },
    { d_dummy_proc,       240,  144+12+12-4,  40,   16,   vc(12),  vc(1),  0,       0,          5,             0,       NULL, NULL, NULL },
    // 20
    { d_dummy_proc,       176,  94-16,   48,   8,    vc(14),  vc(1),  0,       0,          0,             0, (void *) "Position:", NULL, NULL },
    { d_dummy_proc,       217,  94-16,   32,   8,    vc(11),  vc(1),  0,       0,          0,             0,       NULL, NULL, NULL },
    { d_dummy_proc,       176,  104-8,  48,   8,    vc(14),  vc(1),  0,       0,          0,             0, (void *) "Length:", NULL, NULL },
    { d_dummy_proc,       216,  104-8,  32,   8,    vc(11),  vc(1),  0,       0,          0,             0,       NULL, NULL, NULL },
    { d_dummy_proc,       56,   104-8,  48,   8,    vc(14),  vc(1),  0,       0,          0,             0, (void *) "Time:", NULL, NULL },
    { d_dummy_proc,       104,  104-8,  32,   8,    vc(11),  vc(1),  0,       0,          0,             0,       NULL, NULL, NULL },
    // 26
    { d_dummy_proc,      56,   144+12+12,  80+1,   8+1,    vc(14),  vc(1),  0,       0,          1,             0, (void *) "Disable Saving", NULL, NULL },
    { jwin_button_proc,     90,   160+12+12,  61,   21,   vc(14),  vc(1),  'k',     D_EXIT,     0,             0, (void *) "O&K", NULL, NULL },
    { jwin_button_proc,     170,  160+12+12,  61,   21,   vc(14),  vc(1),  27,      D_EXIT,     0,             0, (void *) "Cancel", NULL, NULL },
    { d_keyboard_proc,   0,    0,    0,    0,    0,    0,    0,       0,       KEY_F1,   0, (void *) onHelp, NULL, NULL },
    { NULL,                 0,    0,    0,    0,   0,       0,       0,       0,          0,             0,       NULL,                           NULL,  NULL }
};


void edit_chiptune(int i)
{
    // TO DO : adapt for non-midi formats
    int ret;
    byte flags;
    int id = i;
    int format = 0;
    char title[256] = {0};
    strcpy(title, customchiptunes[i].filename);
  
//  char format_str[8];
//  int format;

    void *data = customchiptunes[i].data;
    
    //if(customchiptunes[i].format == MFORMAT_MIDI) get_midi_info((MIDI*) data,&Midi_Info);
    
   
    flags = customchiptunes[i].flags;
    byte loaded_ok = 0;
    editchiptune_dlg[0].dp2=lfont;
    
    if(is_large)
    {
        large_dialog(editchiptune_dlg);
        editchiptune_dlg[13].dp2 = font;
        editchiptune_dlg[12].dp2 = font;
        editchiptune_dlg[11].dp2 = font;
        editchiptune_dlg[10].dp2 = font;
    }
    
    do
    {
        sprintf(title,"%d",volume);
        sprintf(start_str,"%ld",start);
        sprintf(loop_start_str,"%ld",loop_start);
        sprintf(loop_end_str,"%ld",loop_end);
        sprintf(len_str,"%d",Midi_Info.len_beats);
        sprintf(pos_str,"%ld",midi_pos);
        
        editchiptune_dlg[3].dp = data?(void *) "Loaded":(void *) "Empty";
        editchiptune_dlg[4].dp = data?title:(void *) "Empty";
	
        DIALOG_PLAYER *p = init_dialog(editchiptune_dlg,-1);
        
        while(update_dialog(p))
        {
            custom_vsync();
            scare_mouse();
            //      text_mode(vc(1));
            textprintf_ex(screen,is_large? lfont_l : font,editchiptune_dlg[0].x+int(193*(is_large?1.5:1)),editchiptune_dlg[0].y+int(58*(is_large?1.5:1)),jwin_pal[jcBOXFG],jwin_pal[jcBOX],"%-5ld",midi_pos);
            unscare_mouse();
            //if(zqwin_scale > 1)
            {
                //stretch_blit(screen, hw_screen, 0, 0, screen->w, screen->h, 0, 0, hw_screen->w, hw_screen->h);
            }
            // else
            {
                //blit(screen, hw_screen, 0, 0, 0, 0, screen->w, screen->h);
            }
        }
        
        ret = shutdown_dialog(p);
        
        
        switch(ret)
        {
        case 9:
            if(getname("Load Chiptune","spc;gbs;vgm;gym;nsf",NULL,temppath,true))
            {
                stop_midi();
		char *ext=get_extension(temp.filename);
                if((stricmp(ext,"it")==0) format = CHIPTYPE_IT;
                else if((stricmp(ext,"xm")==0) format = CHIPTYPE_XM;
                else if((stricmp(ext,"sm3")==0) format = CHIPTYPE_SM3;
                else if((stricmp(ext,"mod")==0) format = CHIPTYPE_MOD;
                else if((stricmp(ext,"spc")==0) format = CHIPTYPE_SPC;
                else if((stricmp(ext,"gbs")==0) format = CHIPTYPE_GBS;
                else if((stricmp(ext,"vgm")==0) format = CHIPTYPE_VGM;
                else if((stricmp(ext,"gym")==0) format = CHIPTYPE_GYM;
                else if((stricmp(ext,"nsf")==0) format = CHIPTYPE_NSF;
			
                ////if(data!=NULL && data!=customchiptunes[i].data)
                //{
                //    destroy_midi((MIDI*)data);
                //}

                //packfile_password("");
		    
		/*switch(format)
		{
			//load based on file type
			case CHIPTYPE_IT:
				if((data=dumb_load_it(temppath))!=NULL)
				{
					loaded_ok = 1;
				}
				break;
		
			case CHIPTYPE_XM:
				if((data=dumb_load_xm(temppath))!=NULL)
				{
					loaded_ok = 1;
				}
				break;
			
			case CHIPTYPE_SM3:
				if((data=dumb_load_s3m(temppath))!=NULL)
				{
					loaded_ok = 1;
				}
				break;
				
			case CHIPTYPE_MOD:
				if((data=dumb_load_mod(temppath))!=NULL)
				{
					loaded_ok = 1;
				}
				break;
			
				
			case CHIPTYPE_SPC:
				if((data=gme_load_file(temppath,ext))!=NULL)
				{
					loaded_ok = 1;
				}
				break;
		
			
			default: break;
		}*/

                if((data=gme_load_file(temppath,ext))==NULL)
                {
                    jwin_alert("Error","Error loading tune:",temppath,NULL,"Dang",NULL,13,27,lfont);
                }
                else
                {
                    char *t = get_filename(temppath);
                    int j;
                    
                    for(j=0; j<256 && t[j]!=0 && t[j]!='.'; j++)
                    {
                        title[j]=t[j];
                    }
                    
                    title[j]=0;
                }
                
                //get_midi_info((MIDI*)data,&Midi_Info);
            }
            
            break;
            
        case 10:
	    midi_stop();
            zcmusic_stop(data);
            break;
            
        case 11:
        {
		midi_stop();
		zcmusic_stop(data);
		//gme_play((GMEFILE*) zcm, vol);
		/*switch(format)
		{
			//load based on file type
			case CHIPTYPE_IT:
				if((data=dumb_load_it(temppath))!=NULL)
				{
					loaded_ok = 1;
				}
				break;
		
			case CHIPTYPE_XM:
				if((data=dumb_load_xm(temppath))!=NULL)
				{
					loaded_ok = 1;
				}
				break;
			
			case CHIPTYPE_SM3:
				if((data=dumb_load_s3m(temppath))!=NULL)
				{
					loaded_ok = 1;
				}
				break;
				
			case CHIPTYPE_MOD:
				if((data=dumb_load_mod(temppath))!=NULL)
				{
					loaded_ok = 1;
				}
				break;
				
			case CHIPTYPE_SPC:
				if((data=gme_load_file(temppath,ext))!=NULL)
				{
					loaded_ok = 1;
				}
				break;
		
			
			default: break;
		}
		*/
        }
        break;
        }
    }
    while(ret<26&&ret!=0);
    
    stop_midi();
    
    if(ret==27)
    {
        strcpy(customchiptunes[i].filename,title);
        customchiptunes[i].format = format;
        customchiptunes[i].flags = flags;
        
        if(data!=customchiptunes[i].data)
        {
            if(customchiptunes[i].data)
                destroy_midi((MIDI*)customchiptunes[i].data);
                
            customchiptunes[i].data = data;
        }
        
        saved=false;
    }
    
    if((ret==28||ret==0) && data!=customchiptunes[i].data)
    {
        if(data)
        {
            destroy_midi((MIDI*)data);
        }
    }
}

int d_midilist_proc(int msg,DIALOG *d,int c)
{
    if(msg==MSG_DRAW)
    {
        int i = d->d1;
        int x = d->x+d->w+8;
        int y = d->y+4;
        
        textout_right_ex(screen,font,"Volume:",x+51,y+8+5,jwin_pal[jcBOXFG],jwin_pal[jcBOX]);
        textout_right_ex(screen,font,"Loop:",x+51,y+16+5,jwin_pal[jcBOXFG],jwin_pal[jcBOX]);
        textout_right_ex(screen,font,"Start:",x+51,y+24+5,jwin_pal[jcBOXFG],jwin_pal[jcBOX]);
        textout_right_ex(screen,font,"Loop Start:",x+51,y+32+5,jwin_pal[jcBOXFG],jwin_pal[jcBOX]);
        textout_right_ex(screen,font,"Loop End:",x+51,y+40+5,jwin_pal[jcBOXFG],jwin_pal[jcBOX]);
        
        textprintf_ex(screen,font,x+56,y+8+5,jwin_pal[jcBOXFG],jwin_pal[jcBOX],"%-3d",customchiptunes[i].volume);
        textprintf_ex(screen,font,x+56,y+16+5,jwin_pal[jcBOXFG],jwin_pal[jcBOX],"%s",customchiptunes[i].loop?"On ":"Off");
        textprintf_ex(screen,font,x+56,y+24+5,jwin_pal[jcBOXFG],jwin_pal[jcBOX],"%-5ld",customchiptunes[i].start);
        textprintf_ex(screen,font,x+56,y+32+5,jwin_pal[jcBOXFG],jwin_pal[jcBOX],"%-5ld",customchiptunes[i].loop_start);
        textprintf_ex(screen,font,x+56,y+40+5,jwin_pal[jcBOXFG],jwin_pal[jcBOX],"%-5ld",customchiptunes[i].loop_end);
    }
    
    return jwin_list_proc(msg,d,c);
}

//static ListData custommidi_list(custommidilist, is_large ? &sfont3 : &font);
static ListData custommidi_list(custommidilist, is_large ? &lfont_l : &font);

static DIALOG selectchiptune_dlg[] =
{
    /* (dialog proc)     (x)   (y)   (w)   (h)   (fg)     (bg)    (key)    (flags)     (d1)           (d2)     (dp) */
    { jwin_win_proc,     24,   20,   273,  189,  vc(14),  vc(1),  0,       D_EXIT,          0,             0, (void *) "Select music", NULL, NULL },
    { d_dummy_proc,      160,  56,     0,  8,    vc(15),  vc(1),  0,       0,          0,             0,       NULL, NULL, NULL },
    { d_midilist_proc,   31,   44,   164, (1+16)*8,   jwin_pal[jcTEXTFG],  jwin_pal[jcTEXTBG],  0,       D_EXIT,     0,             0, (void *) &custommidi_list, NULL, NULL },
    // { jwin_droplist_proc, 72-12,   60+4,   161,  16,   jwin_pal[jcTEXTFG],  jwin_pal[jcTEXTBG],  0,       0,     0,             0, (void *) &tracknum_list, NULL, NULL },
    // { jwin_droplist_proc, 72-12,   60+4,   161,  16,   jwin_pal[jcTEXTFG],  jwin_pal[jcTEXTBG],  0,       0,     0,             0, (void *) &custommidi_list, NULL, NULL },
    
    { jwin_button_proc,     90,   160+12+12,  61,   21,   vc(14),  vc(1),  13,     D_EXIT,     0,             0, (void *) "Edit", NULL, NULL },
    { jwin_button_proc,     170,  160+12+12,  61,   21,   vc(14),  vc(1),  27,      D_EXIT,     0,             0, (void *) "Done", NULL, NULL },
    { d_keyboard_proc,   0,    0,    0,    0,    0,       0,      0,       0,          0,             KEY_DEL, (void *) close_dlg, NULL, NULL },
    { d_timer_proc,         0,    0,     0,    0,    0,       0,       0,       0,          0,          0,         NULL, NULL, NULL },
    { NULL,                 0,    0,    0,    0,   0,       0,       0,       0,          0,             0,       NULL,                           NULL,  NULL }
};

int onMidis()
{
    stopMusic();
    int ret;
    char buf[MIDI_TRACK_BUFFER_SIZE];
    number_list_size=MAXcustomchiptunes;
    number_list_zero=false;
    strcpy(temppath,midipath);
    selectchiptune_dlg[0].dp2=lfont;
    
    
    if(is_large)
        large_dialog(selectchiptune_dlg);
        
    selectchiptune_dlg[2].dp2 = 0;
    
    go();
    ret=zc_do_dialog(selectchiptune_dlg,2);
    
    while(ret!=4&&ret!=0)
    {
        int d=selectchiptune_dlg[2].d1;
        
        if(ret==5)
        {
            sprintf(buf,"Delete music %d?",d+1);
            
            if(jwin_alert("Confirm Delete",buf,NULL,NULL,"&Yes","&No",'y','n',lfont)==1)
            {
                customchiptunes[d].reset(); // reset_midi(customMIDIs+d);
                saved=false;
            }
        }
        else
        {
            edit_chiptune(d);
        }
        
        ret=zc_do_dialog(selectchiptune_dlg,2);
    }
    
    comeback();
    return D_O_K;
}