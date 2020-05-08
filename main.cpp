#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<vector>

using namespace sf;
using namespace std;

RenderWindow window(VideoMode(1000,600), "Battleship");

int a,y=400,x1=20,y1=350,pos1,enemy_num,boss_dir=1,enemy_dir[3]={1,1,1},cnt=0,boss_hit=0,level=0,enemy_set=700,ene_cnt[3]={0,0,0};
float enemy_speed=3.f;
bool mve=false,shp=true,hit_me=false,boss=false,enemy=false;

Texture tship,tbackg,tbullet[3],tenemy[10],tboss[4],tmenu,tmenubar[20],twin,tover;
SoundBuffer bufrback,bufrshoot,bufrexplo,bufrboss,bufme_hit,bufme_explod,bufmenu,bufclick,bufwin,bufover;

Sprite ship,backg,Enemy_bullet,boss_bullet,Senemy,My_bullet,Boss,smenu,smenubar[20],swin,sover;
Sound backsound,shootsound,explosion,boss_dead,me_hit,me_explod,menu,click0,win0,over0;

vector<Sprite> My_bullets;
vector<Sprite> Enemy_bullets;
vector<Sprite> boss_bullets;
vector<Sprite> Enemys_nor;
vector<Sprite> Bosses;

void loadimg();
void loadsound();
void My_shoot_play();
void My_shoot_set();
void check_My_hit();
void enemy_shoot_play();
void boss_shoot_play();
void enemy_shoot_set();
void boss_shoot_set();
void check_boss_hit();
void check_enemy_hit();
void enemy_create(int x,int y,int enemy_num);
void enemy_live(int n2);
void boss_live(int n3);
void boss_creat(int x,int y,int l);

Clock boss_clock,My_clock,enemy_clock;

int main()
{
    int My_bullet_delay=2,boss_bullet_delay=2,enemy_shoot_delay=2,m=50,i,ship_mve,dist;
    window.setFramerateLimit(20);
    window.setKeyRepeatEnabled(false);

    loadimg();
    loadsound();
    for(i=0;i<3;++i)
    {
        enemy_create((ship.getPosition().x+200)+m,250+(m+50),level);
        m=m+90;
        Enemy_bullets.push_back(Enemy_bullet);
    }
    boss=false,enemy=true;
    shootsound.setVolume(20);

    //Main menu
    main0:
    menu.play();
    menu.setLoop(true);
    View vw2(Vector2f(500,300),Vector2f(1000.f,600.f));
    dist=0;

    for(i=0;i<5;++i)
    {
        smenubar[i].setPosition(650,250+dist);
        smenubar[i+5].setPosition(650,250+dist);
        dist=dist+50;
    }
    while(window.isOpen())
    {
        window.setView(vw2);
        window.draw(smenu);
        Vector2f mouse0(Mouse::getPosition(window));
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        for(i=0;i<5;++i)
        {
            window.draw(smenubar[i]);
        }
        if(smenubar[0].getGlobalBounds().contains(mouse0))
        {
            window.draw(smenubar[5]);
            if(Mouse::isButtonPressed(Mouse::Left))
            {
                click0.play();
                goto game;
            }
        }
        if(smenubar[1].getGlobalBounds().contains(mouse0))
        {
            window.draw(smenubar[6]);
        }
        if(smenubar[2].getGlobalBounds().contains(mouse0))
        {
            window.draw(smenubar[7]);
        }
        if(smenubar[3].getGlobalBounds().contains(mouse0))
        {
            window.draw(smenubar[8]);
        }
        if(smenubar[4].getGlobalBounds().contains(mouse0))
        {
            window.draw(smenubar[9]);
            if(Mouse::isButtonPressed(Mouse::Left))
            {
                return 0;
            }
        }
        window.display();
    }
    //Game loop
    game:
    View vw1(Vector2f(250,300),Vector2f(500.f,600.f));
    menu.stop();
    backsound.play();
    backsound.setLoop(true);
     //mve=false,shp=true,hit_me=false,boss=false,enemy=false;

    while (window.isOpen())
    {
        window.draw(backg);
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        //My ship movement
        pos1=ship.getPosition().x;
        if(pos1>3500)
        {
            backsound.stop();
            win0.play();
            win0.setLoop(true);
            mve=false;
            goto wins;
        }
        if(Keyboard::isKeyPressed(Keyboard::Up))
        {
            ship_mve=0;
            ship.move(Vector2f(0,-5.f));
        }

        else if(Keyboard::isKeyPressed(Keyboard::Down))
        {
            ship_mve=870;
            ship.move(Vector2f(0,5.f));
        }
        else ship_mve=435;
        if(Keyboard::isKeyPressed(Keyboard::Escape))
        {
            backsound.stop();
            goto main0;
        }

       if(mve==true)
        {
            if(Keyboard::isKeyPressed(Keyboard::Right))
            {
                vw1.move(Vector2f(5.f,0));
                ship.move(Vector2f(5.f,0));
            }
        }

        //Times
        Time time=boss_clock.getElapsedTime(),time2=My_clock.getElapsedTime(),time3=enemy_clock.getElapsedTime();

        boss_bullet_delay=time.asSeconds();
        My_bullet_delay=time2.asSeconds();
        enemy_shoot_delay=time3.asSeconds();

        //Shoot
        if(Enemys_nor.size()!=0 || Bosses.size()!=0)
        {
            if(Keyboard::isKeyPressed(Keyboard::Space))
            {
                if(shp==true)
                {
                    if(My_bullet_delay>=1)
                    {
                        My_bullet.setPosition(ship.getPosition().x+50,ship.getPosition().y+35);
                        My_shoot_set();
                        shootsound.play();
                        My_clock.restart();
                    }
                }
            }
        }
            My_shoot_play();

        //Enemy move
        if(enemy==true)
        {
            enemy_live(enemy_shoot_delay);
            check_My_hit();
            check_enemy_hit();
            if(Enemys_nor.size()==0)
            {
                mve=true;
            }
        }
        if(ship.getPosition().x==enemy_set)
        {
            mve= !mve;
            boss= !boss;
            enemy = !enemy;
           if(boss==true)
           {
               boss_creat(ship.getPosition().x+300,300,level);
           }
           if(enemy==true)
           {
               m=50;
               for(i=0;i<3;++i)
                {
                    enemy_create((ship.getPosition().x+200)+m,250+(m+50),level);
                    m=m+90;
                    Enemy_bullets.push_back(Enemy_bullet);
                }
           }
            enemy_set=enemy_set+680;
        }
        //boss move
        if(boss==true)
        {
            boss_live(boss_bullet_delay);
            check_boss_hit();
            check_My_hit();
        }

        enemy_shoot_play();
        boss_shoot_play();

        y=ship.getPosition().y;

        //My ship area
        if(y<290)
        {
            ship.setPosition(ship.getPosition().x,290);
        }
        if(y>550)
        {
            ship.setPosition(ship.getPosition().x,550);
        }

        //image draw
        window.setView(vw1);
        if(shp==true&&cnt<=3)
        {
            ship.setTextureRect(IntRect(0,ship_mve,1400,435));
            window.draw(ship);
        }
        else if(shp==false)
        {
            backsound.stop();
            over0.play();
            over0.setLoop(true);
            goto over;
        }
        window.display();
    }

    //Win screen
   wins: while(window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        window.setView(vw2);
        window.draw(swin);
        window.display();
        if(Keyboard::isKeyPressed(Keyboard::Return))
        {
            goto main0;
        }
        else if(Keyboard::isKeyPressed(Keyboard::Escape))
        {
            return 0;
        }
    }
    //game over

    over:while(window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        if(Keyboard::isKeyPressed(Keyboard::Return))
        {
            goto main0;
        }
        else if(Keyboard::isKeyPressed(Keyboard::Escape))
        {
            return 0;
        }
        window.setView(vw2);
        window.draw(sover);
        window.display();
    }
    return 0;
}
void loadimg()
{
    int i;
    if(!tbackg.loadFromFile("img\\background1.jpg"))
    {
        cout<<"Error"<<endl;
    }
    if(!tship.loadFromFile("img\\ship_all.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tbullet[0].loadFromFile("img\\Bullet.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tbullet[1].loadFromFile("img\\enbullet.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tboss[0].loadFromFile("img\\Boss1.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tboss[1].loadFromFile("img\\Boss2.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tboss[2].loadFromFile("img\\Boss3.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tenemy[0].loadFromFile("img\\enemy1.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tenemy[1].loadFromFile("img\\enemy2.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tenemy[2].loadFromFile("img\\enemy3.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tenemy[3].loadFromFile("img\\blust_enemy1.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tenemy[4].loadFromFile("img\\blust_ene2.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tenemy[5].loadFromFile("img\\blust_ene3.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tmenu.loadFromFile("img\\menu_back.jpg"))
    {
        cout<<"Error"<<endl;
    }
    if(!tmenubar[0].loadFromFile("img\\newgame0.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tmenubar[1].loadFromFile("img\\load0.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tmenubar[2].loadFromFile("img\\instruction0.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tmenubar[3].loadFromFile("img\\about0.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tmenubar[4].loadFromFile("img\\exit0.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tmenubar[5].loadFromFile("img\\newgame.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tmenubar[6].loadFromFile("img\\load.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tmenubar[7].loadFromFile("img\\instruction.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tmenubar[8].loadFromFile("img\\about.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!tmenubar[9].loadFromFile("img\\exit.png"))
    {
        cout<<"Error"<<endl;
    }
    if(!twin.loadFromFile("img\\win.jpg"))
    {
        cout<<"Error"<<endl;
    }
    if(!tover.loadFromFile("img\\game_over.jpg"))
    {
        cout<<"Error"<<endl;
    }
    for(i=0;i<=9;++i)
    {
        smenubar[i].setTexture(tmenubar[i]);
    }
    sover.setTexture(tover);
    swin.setTexture(twin);
    smenu.setTexture(tmenu);
    ship.setTexture(tship);
    backg.setTexture(tbackg);
    boss_bullet.setTexture(tbullet[1]);
    backg.setScale(0.7f,0.3f);
    ship.setScale(0.05f,0.15f);
    ship.setPosition(x1,y1);
}

void loadsound()
{
    if(!bufrshoot.loadFromFile("sound\\gun-gunshot-01.wav"))
    {
        cout<<"error loading\n";
    }
    if(!bufrback.loadFromFile("sound\\backsound.wav"))
    {
        cout<<"error loading\n";
    }
    if(!bufrexplo.loadFromFile("sound\\explosion.wav"))
    {
        cout<<"error loading\n";
    }
    if(!bufrboss.loadFromFile("sound\\boss_explod.wav"))
    {
        cout<<"error loading\n";
    }
    if(!bufme_hit.loadFromFile("sound\\me2_hit.wav"))
    {
        cout<<"error loading\n";
    }
    if(!bufme_explod.loadFromFile("sound\\me_explod.wav"))
    {
        cout<<"error loading\n";
    }
    if(!bufmenu.loadFromFile("sound\\menu_sound.wav"))
    {
        cout<<"error loading\n";
    }
    if(!bufclick.loadFromFile("sound\\click.wav"))
    {
        cout<<"error loading\n";
    }
    if(!bufwin.loadFromFile("sound\\win.wav"))
    {
        cout<<"error loading\n";
    }
    if(!bufover.loadFromFile("sound\\over.wav"))
    {
        cout<<"error loading\n";
    }
    win0.setBuffer(bufwin);
    over0.setBuffer(bufover);
    click0.setBuffer(bufclick);
    menu.setBuffer(bufmenu);
    me_explod.setBuffer(bufme_explod);
    me_hit.setBuffer(bufme_hit);
    boss_dead.setBuffer(bufrboss);
    explosion.setBuffer(bufrexplo);
    backsound.setBuffer(bufrback);
    shootsound.setBuffer(bufrshoot);
}

void enemy_create(int x,int y,int enemy_num)
{
    Senemy.setPosition(x,y);
    Senemy.setTexture(tenemy[enemy_num]);
    Enemys_nor.push_back(Senemy);
}
void enemy_live(int n2)
{
    int i;
    for(i=0;i<Enemys_nor.size();++i)
    {
        window.draw(Enemys_nor[i]);
        Enemys_nor[i].move(0,enemy_dir[i]*5.f);
        Enemy_bullet.setPosition(Enemys_nor[i].getPosition().x,Enemys_nor[i].getPosition().y);
        if(n2>=2)
        {
            enemy_shoot_set();
            shootsound.play();
            enemy_clock.restart();
        }
        if(Enemys_nor[i].getPosition().y<300)
        {
            enemy_dir[i]=1;
        }
        else if(Enemys_nor[i].getPosition().y>560)
        {
            enemy_dir[i]=-1;
        }
    }
}

void boss_live(int n3)
{
        if(Bosses.size()!=0)
        {
            window.draw(Bosses[0]);
            Bosses[0].move(0,boss_dir*5.f);
            boss_bullet.setPosition(Bosses[0].getPosition().x,Bosses[0].getPosition().y);
            if(n3>=1)
            {
                boss_shoot_set();
                shootsound.play();
                boss_clock.restart();
            }
            if(Bosses[0].getPosition().y<300)
            {
                boss_dir=1;
            }
            else if(Bosses[0].getPosition().y>550)
            {
                boss_dir=-1;
            }
        }
}

void boss_creat(int x,int y,int l)
{
    Boss.setPosition(x,y);
    Boss.setScale(0.05f,0.1f);
    Boss.setTexture(tboss[l]);
    Bosses.push_back(Boss);
}

void enemy_shoot_set()
{
    Enemy_bullets.push_back(Enemy_bullet);
}

void boss_shoot_set()
{
    boss_bullets.push_back(boss_bullet);
}

void boss_shoot_play()
{
    int i;
    for(i=0;i<boss_bullets.size();i++)
    {
        boss_bullets[i].setTexture(tbullet[1]);
        window.draw(boss_bullets[i]);
        boss_bullets[i].move(-8.f,0);
        if(boss_bullets[i].getPosition().x<=Bosses[0].getPosition().x-500)
        {
            boss_bullets.erase(boss_bullets.begin()+i);
        }
    }
}
void enemy_shoot_play()
{
    int i;
    for(i=0;i<Enemy_bullets.size();i++)
    {
        Enemy_bullets[i].setTexture(tbullet[1]);
        window.draw(Enemy_bullets[i]);
        Enemy_bullets[i].move(-8.f,0);
        if(Enemy_bullets[i].getPosition().x<=Enemys_nor[i].getPosition().x-500)
        {
            Enemy_bullets.erase(Enemy_bullets.begin()+i);
        }
    }
}
void check_My_hit()
{
    int i,j;
    for(i=0;i<My_bullets.size();++i)
    {
        if(enemy==true)
        {
            for(j=0;j<Enemys_nor.size();++j)
            {
                if(My_bullets[i].getGlobalBounds().intersects(Enemys_nor[j].getGlobalBounds()))
                {
                    explosion.play();
                    My_bullets.erase(My_bullets.begin()+i);
                    Enemy_bullets.erase(Enemy_bullets.begin()+i);
                    ++ene_cnt[j];
                    a=(level-1)+2;
                    cout<<a<<"ene "<<ene_cnt[j]<<endl;
                    if(a<0)
                    {
                        a=(-a);
                    }
                    if(ene_cnt[j]== a )
                    {
                        Senemy.setTexture(tenemy[level+3]);
                        Senemy.setPosition(Enemys_nor[j].getPosition());
                        Enemys_nor[j]=Senemy;
                    }
                    if(ene_cnt[j]>(level+1))
                    {
                        Enemys_nor.erase(Enemys_nor.begin()+j);
                        ene_cnt[j]=0;
                    }
                }
            }
        }
        else if(boss==true)
        {
            if(My_bullets[i].getGlobalBounds().intersects(Bosses[0].getGlobalBounds()))
            {
                My_bullets.erase(My_bullets.begin()+i);
                ++boss_hit;
                if(boss_hit<(level+5))
                {
                    explosion.play();
                }
                if(boss_hit>=(level+5))
                {
                    boss_dead.play();
                    Bosses.erase(Bosses.begin()+i);
                    boss_hit=0;
                    mve=true;
                    ++level;
                }
            }
        }
    }
}

void check_enemy_hit()
{
    int i;
    for(i=0;i<Enemy_bullets.size();++i)
    {
        if(Enemy_bullets[i].getGlobalBounds().intersects(ship.getGlobalBounds()))
        {
            ++cnt;
            if(cnt<3)
            {
                me_hit.play();
            }
            Enemy_bullets.erase(Enemy_bullets.begin()+i);
            if(cnt>=4)
            {
                me_explod.play();
                hit_me=true;
                shp=false;
                cnt=0;
            }
        }
    }
}
void check_boss_hit()
{
    int i;
    for(i=0;i<boss_bullets.size();++i)
    {
        if(boss_bullets[i].getGlobalBounds().intersects(ship.getGlobalBounds()))
        {
            cnt+=2;
            if(cnt<3)
            {
                me_hit.play();
            }
            if(cnt>=4)
            {
                me_explod.play();
                boss_bullets.erase(boss_bullets.begin()+i);
                hit_me=true;
                shp=false;
            }
        }
    }
}

void My_shoot_set()
{
    My_bullets.push_back(My_bullet);
}

void My_shoot_play()
{
    int i;
    for(i=0;i<My_bullets.size();i++)
    {
        window.draw(My_bullets[i]);
        My_bullets[i].setTexture(tbullet[0]);
        My_bullets[i].move(10.f,0);
        if(My_bullets[i].getPosition().x>=ship.getPosition().x+470)
        {
            My_bullets.erase(My_bullets.begin()+i);
        }
    }
}

