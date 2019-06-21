#ifndef Display_h
#define Display_h

#include <stdint.h>
#define LCD_FPS 10

enum disp{DISP_MAIN, DISP_MENU, DISP_EXT, DISP_NR};

class Display_class{  
public:
  uint8_t   Page;
  uint8_t   OldPage;
  uint8_t   ItemsPerPage;
  
  uint64_t  OldRefreshTime;
  uint64_t  RefreshMs;
  bool      PendUpd;
  uint8_t   DispMode;
  uint8_t   OldMode;
  bool      CycTimeUpdated = 0;
  bool      HeatFanUpdated = 0;
  
  void Init(void);
  void Refresh(void);
  void SetMode(uint8_t mode);
  void SetState(uint8_t state);
  void ShowSplash(void);
  void Scroll(int result);
  void Tune(int result);
  void SwitchModes(void);
  
};

extern Display_class Display;
#endif
