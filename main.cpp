#include <iostream>
#include<SDL.h>
using namespace std;

const int height_of_screen=680;
const int width_of_screen=1080;
const int button_width=415/3*2;
const int button_height=125/3*2;
const int button_total=1;
//
enum button_event{
     button_event_mouse_out=0,
     button_event_mouse_over_motion=1,
     button_event_mouse_down=2,
     button_event_mouse_up=3,
     button_event_mouse_total=4
};
SDL_Window*window=NULL;
SDL_Renderer*renderer=NULL;
int start_button_frame=0;
const int quality_of_start_button_frame=2;
SDL_Rect start_button_clip[quality_of_start_button_frame];
bool inside_start_button=false;

int tutorial_button_frame=0;
const int quality_of_tutorial_button_frame=2;
SDL_Rect tutorial_button_clip[quality_of_tutorial_button_frame];
bool inside_tutorial_button=false;

int quit_button_frame=0;
const int quality_of_quit_button_frame=2;
SDL_Rect quit_button_clip[quality_of_quit_button_frame];
bool inside_quit_button=false;

//******************************************************************//
class texture
{
    public:
        texture();
        ~texture();
        bool loadFromFile(string path );
        void free();
        void render(SDL_Rect*clip);
        void setWidth(int w);
        void setHeight(int h);
        void settdx(double x);
        void settdy(double y);
        int getheight();
        int getwidth();
        double gettdx();
        double gettdy();

    private:
        //The actual hardware texture
        SDL_Texture* mTexture;

        //Image dimensions
        int mWidth;
        int mHeight;
        double tdx,tdy;
};
texture::texture(){
         mTexture=NULL;
         mWidth=0;
         mHeight=0;
         tdx=0;
         tdy=0;
}
void texture::free(){
   SDL_DestroyTexture(mTexture);
}
texture::~texture(){
    free();
}
bool texture::loadFromFile(string s){
     bool success=true;
     SDL_Surface*loadedimage=SDL_LoadBMP(s.c_str());
     if(loadedimage==NULL){
        success=false;
        cout<<"error"<<endl;
     }
     else{
         SDL_SetColorKey(loadedimage,SDL_TRUE,SDL_MapRGB(loadedimage->format,255,0,0));
         mTexture=SDL_CreateTextureFromSurface(renderer,loadedimage);
         if(mTexture==NULL){
            cout<<"error"<<endl;
            success=false;
         }
     }
     SDL_FreeSurface(loadedimage);
     return success;
}

void texture :: render(SDL_Rect*clip){
     SDL_Rect position_and_Size;
     position_and_Size.x=tdx;
     position_and_Size.y=tdy;
     position_and_Size.h=mHeight;
     position_and_Size.w=mWidth;
     SDL_RenderCopy(renderer,mTexture,clip,&position_and_Size);
}
void texture::setHeight(int h){
     mHeight=h;
}
void texture::setWidth(int w){
     mWidth=w;
}
void texture::settdx(double x){
     tdx=x;
}
void texture::settdy(double y){
     tdy=y;
}
int texture::getheight(){
     return mHeight;
}
int texture::getwidth(){
     return mWidth;
}
double texture::gettdx(){
     return tdx;
}
double texture::gettdy(){
     return tdy;
}
//***************************************************************************//
// lớp con chuột
texture image_start;
texture image_tutorial;
texture image_quit;

const int quality_of_menu_background_sprite=2;
int menu_background_frame=0;
SDL_Rect menu_background_clip[quality_of_menu_background_sprite];
texture menu_background;

texture sans_in_bg;
bool repeat_moving_of_sans=false;
texture soul_of_sans_in_bg;
bool repeat_moving_of_soul_of_sans=false;
bool can_soul_of_sans_appear=false;

texture chara_in_bg;
bool repeat_moving_of_chara=false;
texture soul_of_chara_in_bg;
bool repeat_moving_of_soul_of_chara=false;
bool can_soul_of_chara_appear=false;

const int quality_of_bonus_sprite1=3;
SDL_Rect bonus_sprite1_for_button_clip[quality_of_bonus_sprite1];
int bonus_sprite1_for_button_frame=0;
texture bonus_sprite1_for_button;
bool is_bonus_sprite1_for_start_button_active=false;
int time_appearing_of_bonus_spr1_for_start_button=0;
bool is_bonus_sprite1_for_tutorial_button_active=false;
int time_appearing_of_bonus_spr1_for_tutorial_button=0;
bool is_bonus_sprite1_for_end_button_active=false;
texture bonus_sprite2_for_button;
bool is_bonus_sprite2_for_start_button_active=false;
bool is_bonus_sprite2_for_tutorial_button_active=false;


class button
{
    public:
        // hàm khởi tạo giá trị mặc định
        button();

        // hàm thiết lập vị trí
        void setPosition( int x, int y);

        //hàm quản lý vòng lặp sự kiện
        void handleEvent( SDL_Event* e );


        int gettdx();
        int gettdy();

    private:
        //vị trí của con trỏ chuột
        SDL_Point position;

        //
        button_event current_sprite;
};
button::button(){
       position.x=0;
       position.y=0;
       current_sprite=button_event_mouse_out;
}
void button::setPosition(int x,int y){
     position.x=x;
     position.y=y;
}
int tdx_of_my_mouse=0;
int tdy_of_my_mouse=0;



int button :: gettdx(){
    return position.x;
}
int button :: gettdy(){
    return position.y;
}
button start_button_system[button_total];
button tutorial_button_system[button_total];
button quit_button_system[button_total];
//************************************************************************//
bool init(){
     bool success=true;
     if(SDL_Init(SDL_INIT_VIDEO)<0){
        success=false;
     }
     else{
         window=SDL_CreateWindow("lesson 17",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width_of_screen,height_of_screen,SDL_WINDOW_SHOWN);
         if(window==NULL){
            cout<<"error"<<endl;
            success=false;
         }
         else{
             renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
         }
     }
   return success;
}
bool loadmedia(){
     bool success=true;
     if(!image_start.loadFromFile("start_button_status.bmp")){
        cout<<"error"<<endl;
        success=false;
     }
else
	{

	    menu_background_clip[0].h=454;
	    menu_background_clip[0].w=811;
	    menu_background_clip[0].x=0;
	    menu_background_clip[0].y=0;

        menu_background_clip[1].h=454;
	    menu_background_clip[1].w=811;
	    menu_background_clip[1].x=811;
	    menu_background_clip[1].y=0;


	    start_button_clip[0].h=365;
	    start_button_clip[0].w=975;
	    start_button_clip[0].x=0;
	    start_button_clip[0].y=0;

	    start_button_clip[1].h=365;
	    start_button_clip[1].w=975;
	    start_button_clip[1].x=0;
	    start_button_clip[1].y=365+270;



		start_button_system[0].setPosition(width_of_screen/2-button_width/2,height_of_screen/2-button_height/2);
		image_start.settdx(-start_button_system[0].gettdx());
		image_start.settdy(start_button_system[0].gettdy());
		image_start.setHeight(button_height);
		image_start.setWidth(button_width);

		image_tutorial.loadFromFile("tutorial_button_status.bmp");
        tutorial_button_clip[0].h=365;
	    tutorial_button_clip[0].w=975;
	    tutorial_button_clip[0].x=0;
	    tutorial_button_clip[0].y=0;

	    tutorial_button_clip[1].h=365;
	    tutorial_button_clip[1].w=975;
	    tutorial_button_clip[1].x=0;
	    tutorial_button_clip[1].y=365+270;

	    tutorial_button_system[0].setPosition(width_of_screen/2-button_width/2,height_of_screen/2-button_height/2+150);
		image_tutorial.settdx(-tutorial_button_system[0].gettdx());
		image_tutorial.settdy(tutorial_button_system[0].gettdy());
		image_tutorial.setHeight(button_height);
		image_tutorial.setWidth(button_width);


	    image_quit.loadFromFile("quit_button_status.bmp");
        quit_button_clip[0].h=365;
        quit_button_clip[0].w=975;
	    quit_button_clip[0].x=0;
	    quit_button_clip[0].y=0;

	    quit_button_clip[1].h=365;
	    quit_button_clip[1].w=975;
	    quit_button_clip[1].x=0;
	    quit_button_clip[1].y=365+270;



		quit_button_system[0].setPosition(width_of_screen/2-button_width/2,height_of_screen/2-button_height/2+300);
		image_quit.settdx(-quit_button_system[0].gettdx());
		image_quit.settdy(quit_button_system[0].gettdy());
		image_quit.setHeight(button_height);
		image_quit.setWidth(button_width);



       // back ground

		menu_background.loadFromFile("background_of_menu.bmp");
		menu_background.setHeight(height_of_screen);
		menu_background.setWidth(width_of_screen);
		menu_background.settdx(0);
		menu_background.settdy(0);

        sans_in_bg.loadFromFile("sans_in_bg.bmp");
        sans_in_bg.setHeight(640);
		sans_in_bg.setWidth(640);
        sans_in_bg.settdx(0-640);
		sans_in_bg.settdy(height_of_screen);

        soul_of_sans_in_bg.loadFromFile("soul_of_sans.bmp");
        soul_of_sans_in_bg.setHeight(640);
		soul_of_sans_in_bg.setWidth(640);
        soul_of_sans_in_bg.settdx(sans_in_bg.gettdx()+20);
		soul_of_sans_in_bg.settdy(sans_in_bg.gettdy()-100);


        chara_in_bg.loadFromFile("chara_in_bg.bmp");
        chara_in_bg.setHeight(286);
		chara_in_bg.setWidth(442);
        chara_in_bg.settdx(width_of_screen);
		chara_in_bg.settdy(0-286);

        soul_of_chara_in_bg.loadFromFile("soul_of_chara.bmp");
        soul_of_chara_in_bg.setHeight(640);
		soul_of_chara_in_bg.setWidth(640);
        soul_of_chara_in_bg.settdx(chara_in_bg.gettdx()-100);
		soul_of_chara_in_bg.settdy(chara_in_bg.gettdy()+100);

		bonus_sprite1_for_button.loadFromFile("bonus_for_button.bmp");
		bonus_sprite1_for_button.setHeight(45);
		bonus_sprite1_for_button.setWidth(57);

		for(int i=0;i<quality_of_bonus_sprite1;i++){
            bonus_sprite1_for_button_clip[i].h=90;
            bonus_sprite1_for_button_clip[i].w=114;
            bonus_sprite1_for_button_clip[i].x=0;
            bonus_sprite1_for_button_clip[i].y=180-90*i;
		}






	}


     return success;
}

//**********************************************************//
int main(int arcg,char*argv[])
{   bool quit=false;
    SDL_Event e;
    if(!init()){cout<<"error";}
    else{
        if(!loadmedia()){ cout<<"error"<<endl;}
        else{
    while( !quit )
            {   SDL_RenderClear(renderer);
                menu_background.render(&menu_background_clip[menu_background_frame]);
                if((SDL_GetTicks()/1000%5==0||SDL_GetTicks()/1000%5==1) && SDL_GetTicks()>0){
                   can_soul_of_chara_appear=true;
                   can_soul_of_sans_appear=true;
                   menu_background_frame=1;
                   menu_background.render(&menu_background_clip[menu_background_frame]);
                }
                else{
                     can_soul_of_chara_appear=false;
                     can_soul_of_sans_appear=false;
                    menu_background_frame=0;
                }
                sans_in_bg.render(NULL);
                if(can_soul_of_sans_appear){soul_of_sans_in_bg.render(NULL);}
                chara_in_bg.render(NULL);
                if(can_soul_of_chara_appear){soul_of_chara_in_bg.render(NULL);}
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                     if(e.type==SDL_MOUSEBUTTONDOWN||e.type==SDL_MOUSEBUTTONUP||e.type==SDL_MOUSEMOTION &&
                        !(image_quit.gettdx()<width_of_screen/2-button_width/2 ||
                        image_start.gettdx()<width_of_screen/2-button_width/2||
                        image_tutorial.gettdx()<width_of_screen/2-button_width/2)){

                        SDL_GetMouseState(&tdx_of_my_mouse,&tdy_of_my_mouse);
      //Mouse is left of the button
                        if(tdx_of_my_mouse>=start_button_system->gettdx() && start_button_system->gettdx()+button_width>=tdx_of_my_mouse && start_button_system->gettdy()<=tdy_of_my_mouse && start_button_system->gettdy()+button_height>=tdy_of_my_mouse){
                            inside_start_button=true;
                        }
                        else{inside_start_button=false;}
                         if(tdx_of_my_mouse>=tutorial_button_system->gettdx() && tutorial_button_system->gettdx()+button_width>=tdx_of_my_mouse && tutorial_button_system->gettdy()<=tdy_of_my_mouse && tutorial_button_system->gettdy()+button_height>=tdy_of_my_mouse){
                            inside_tutorial_button=true;
                        }
                        else{inside_tutorial_button=false;}
                        if(tdx_of_my_mouse>=quit_button_system->gettdx() &&
                           quit_button_system->gettdx()+button_width>=tdx_of_my_mouse
                           && quit_button_system->gettdy()<=tdy_of_my_mouse
                           && quit_button_system->gettdy()+button_height>=tdy_of_my_mouse){
                            inside_quit_button=true;
                        }
                        else{inside_quit_button=false;}
        //Mouse is outside button
                        if( inside_start_button )
                        {      is_bonus_sprite1_for_start_button_active=true;

                               switch( e.type )
                                {
                                case SDL_MOUSEMOTION:
                                start_button_frame=1;
                                break;
                                case SDL_MOUSEBUTTONUP :
                                is_bonus_sprite2_for_start_button_active=true;
                                break;


                                }

                        }
                        else{
                            is_bonus_sprite1_for_start_button_active=false;
                             start_button_frame=0;
                        }
        //Mouse is inside button


                          if( inside_tutorial_button )
                        {

                               is_bonus_sprite1_for_tutorial_button_active=true;
                            switch( e.type )
                                {
                                case SDL_MOUSEMOTION:
                                tutorial_button_frame=1;
                                break;
                                case SDL_MOUSEBUTTONUP:
                               // is_bonus_sprite2_for_tutorial_button_active=true;
                                break;


                                }
                        }
                        else{
                             is_bonus_sprite1_for_tutorial_button_active=false;
                             tutorial_button_frame=0;
                        }
        //Mouse is inside button



                        if( inside_quit_button )
                        {   is_bonus_sprite1_for_end_button_active=true;

                            switch( e.type )
                                {
                                case SDL_MOUSEMOTION:
                                quit_button_frame=1;
                                break;
                                case SDL_MOUSEBUTTONUP:
                                quit=true;
                                break;


                        }
                        }
                        else{
                             is_bonus_sprite1_for_end_button_active=false;
                             quit_button_frame=0;
                        }
        //Mouse is inside button

                }

                    //Handle button events

                     }



                //Render buttons




                //set 3 button start,tutorial,quit moving

                if(SDL_GetTicks()/1000>=2){
                  if(image_quit.gettdx()<width_of_screen/2-button_width/2 ||
                     image_start.gettdx()<width_of_screen/2-button_width/2||
                     image_tutorial.gettdx()<width_of_screen/2-button_width/2){
                     image_start.settdx(image_start.gettdx()+15);
                     image_tutorial.settdx(image_tutorial.gettdx()+15);
                     image_quit.settdx(image_quit.gettdx()+15);

                  }


                    image_start.render(&start_button_clip[start_button_frame]);
                    image_tutorial.render(&tutorial_button_clip[tutorial_button_frame]);
                    image_quit.render(&quit_button_clip[quit_button_frame]);
                }
                else{
                     image_quit.settdx(-quit_button_system[0].gettdx());
                     image_start.settdx(-start_button_system[0].gettdx());
                     image_tutorial.settdx(-tutorial_button_system[0].gettdx());
                }



                // set bonus sprite active when mouse inside button
                if(is_bonus_sprite1_for_end_button_active && !is_bonus_sprite2_for_start_button_active &&
                   !is_bonus_sprite2_for_tutorial_button_active){
                    bonus_sprite1_for_button.settdx(quit_button_system[0].gettdx()-114/2);
                    bonus_sprite1_for_button.settdy(quit_button_system[0].gettdy()+(button_height-45)/2);
                    bonus_sprite1_for_button.render(&bonus_sprite1_for_button_clip[bonus_sprite1_for_button_frame]);

                }
                if(is_bonus_sprite1_for_start_button_active&& !is_bonus_sprite2_for_start_button_active &&
                   !is_bonus_sprite2_for_tutorial_button_active){
                    bonus_sprite1_for_button.settdx(start_button_system[0].gettdx()-114/2);
                    bonus_sprite1_for_button.settdy(start_button_system[0].gettdy()+(button_height-45)/2);
                    bonus_sprite1_for_button.render(&bonus_sprite1_for_button_clip[bonus_sprite1_for_button_frame]);
                    time_appearing_of_bonus_spr1_for_start_button=SDL_GetTicks();

                }
                if(is_bonus_sprite1_for_tutorial_button_active&& !is_bonus_sprite2_for_start_button_active &&
                   !is_bonus_sprite2_for_tutorial_button_active){
                    bonus_sprite1_for_button.settdx(tutorial_button_system[0].gettdx()-114/2);
                    bonus_sprite1_for_button.settdy(tutorial_button_system[0].gettdy()+(button_height-45)/2);
                    bonus_sprite1_for_button.render(&bonus_sprite1_for_button_clip[bonus_sprite1_for_button_frame]);
                    time_appearing_of_bonus_spr1_for_tutorial_button=SDL_GetTicks();
                }




                // set bonus sprite active when mouse up or down
                 if(is_bonus_sprite2_for_start_button_active){
                    bonus_sprite1_for_button.settdx(bonus_sprite1_for_button.gettdx()-(SDL_GetTicks()-time_appearing_of_bonus_spr1_for_start_button)/100);
                    bonus_sprite1_for_button.settdy(start_button_system[0].gettdy()+(button_height-45)/2);
                    if(bonus_sprite1_for_button_frame/10>=3){
                       bonus_sprite1_for_button_frame=20;
                    }
                    bonus_sprite1_for_button.render(&bonus_sprite1_for_button_clip[bonus_sprite1_for_button_frame/10]);
                    bonus_sprite1_for_button_frame++;
                    if(bonus_sprite1_for_button.gettdx()<=0){
                       is_bonus_sprite2_for_start_button_active=false;
                       bonus_sprite1_for_button_frame=0;
                       bonus_sprite1_for_button.settdx(start_button_system[0].gettdx()-114/2);
                       bonus_sprite1_for_button.settdy(quit_button_system[0].gettdy()+(button_height-45)/2);
                    }
                }

                SDL_RenderPresent( renderer );

                //sans in br moving
                if(!repeat_moving_of_sans){
                  if(!(sans_in_bg.gettdx()>=0-20 || sans_in_bg.gettdy()<=height_of_screen-640+15) ){
                        sans_in_bg.settdx(sans_in_bg.gettdx()+5);
                        sans_in_bg.settdy(sans_in_bg.gettdy()-5);

                  }
                  else{
                      repeat_moving_of_sans=true;

                  }
                }



                else{
                    if(SDL_GetTicks()/1000%2==0){
                        sans_in_bg.settdx(sans_in_bg.gettdx()-1);
                        sans_in_bg.settdy(sans_in_bg.gettdy()+1);

                    }
                    else{
                         sans_in_bg.settdx(sans_in_bg.gettdx()+1);
                         sans_in_bg.settdy(sans_in_bg.gettdy()-1);
                    }

                }


                // soul of sans  moving


               if(!repeat_moving_of_soul_of_sans){
                  if(!(soul_of_sans_in_bg.gettdx()>=0+20-15 || soul_of_sans_in_bg.gettdy()<=height_of_screen-640-100+15) ){
                        soul_of_sans_in_bg.settdx(soul_of_sans_in_bg.gettdx()+5);
                        soul_of_sans_in_bg.settdy(soul_of_sans_in_bg.gettdy()-5);

                  }
                  else{
                      repeat_moving_of_soul_of_sans=true;

                  }
                }


                else{
                    if(SDL_GetTicks()/1000%2==0){
                        soul_of_sans_in_bg.settdx(soul_of_sans_in_bg.gettdx()-1);
                        soul_of_sans_in_bg.settdy(soul_of_sans_in_bg.gettdy()+1);

                    }
                    else{
                         soul_of_sans_in_bg.settdx(soul_of_sans_in_bg.gettdx()+1);
                         soul_of_sans_in_bg.settdy(soul_of_sans_in_bg.gettdy()-1);
                    }

                }






                 // chara in br moving

                if(!repeat_moving_of_chara){
                  if(!(chara_in_bg.gettdx()<=width_of_screen-442+50 || chara_in_bg.gettdy()>=0) ){
                        chara_in_bg.settdx(chara_in_bg.gettdx()-442/100+50/100);
                        chara_in_bg.settdy(chara_in_bg.gettdy()+286/100);

                  }
                  else{
                      repeat_moving_of_chara=true;
                      chara_in_bg.settdy(0-50);

                  }
                }


                else{
                    if(SDL_GetTicks()/1000%2==0){
                        chara_in_bg.settdx(chara_in_bg.gettdx()-1);
                        chara_in_bg.settdy(chara_in_bg.gettdy()+1);

                    }
                    else{
                         chara_in_bg.settdx(chara_in_bg.gettdx()+1);
                         chara_in_bg.settdy(chara_in_bg.gettdy()-1);
                    }

                }




                //soul of chara moving
                if(!repeat_moving_of_soul_of_chara){
                    if(!(soul_of_chara_in_bg.gettdx()<=width_of_screen-442-100 || soul_of_chara_in_bg.gettdy()>=100) ){
                        soul_of_chara_in_bg.settdx(soul_of_chara_in_bg.gettdx()-442/100);
                        soul_of_chara_in_bg.settdy(soul_of_chara_in_bg.gettdy()+286/100);

                    }
                    else{
                        repeat_moving_of_soul_of_chara=true;

                    }
                }


                else{
                    if(SDL_GetTicks()/1000%2==0){
                        soul_of_chara_in_bg.settdx(soul_of_chara_in_bg.gettdx()-1);
                        soul_of_chara_in_bg.settdy(soul_of_chara_in_bg.gettdy()+1);

                    }
                    else{
                         soul_of_chara_in_bg.settdx(soul_of_chara_in_bg.gettdx()+1);
                         soul_of_chara_in_bg.settdy(soul_of_chara_in_bg.gettdy()-1);
                    }

                }



            }
        }
    }

    return 0;
}
