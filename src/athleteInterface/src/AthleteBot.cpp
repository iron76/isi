// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2007 RobotCub Consortium
 * Authors: Paul Fitzpatrick
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#include "AthleteBot.h"

#include <yarp/sig/all.h>
#include <yarp/sig/ImageFile.h>
#include <yarp/os/all.h>


using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::sig::draw;
using namespace yarp::sig::file;
using namespace yarp::dev;

#define MAXRND 50000
int rnds[MAXRND];

void AthleteBot::init() {
    int m_w = 640;
    int m_h = 480;
    back.resize(m_w,m_h);
    PixelRgb w(255,255,255);
    PixelRgb r(128,128,255);
    PixelRgb b(0,0,255);
    IMGFOR(back,x,y) {
        back(x,y) = r;
        if (y>=m_h*0.75) {
            back(x,y) = b;
        }
    }
    for (int i=0; i<=m_w; i+=m_w/10) {
        addCircle(back,b,i,int(m_h*0.77),30);
    }
    for (int j=0; j<40; j++) {
        int rr = 30;
        int xx = m_w/2 + Random::uniform(-m_w/4,m_w/4);
        int yy = int(m_h*0.2) - Random::uniform(-int(m_h*0.2),rr);
        addCircle(back,w,xx,yy,rr);
    }
    m_x = (back.width()-this->m_w)/2;
    m_y = (back.height()-this->m_h)/2;
    m_dx = m_dy = 0;
    for (int k=0; k<MAXRND; k++) {
        rnds[k] = int(Random::normal(0,255));
    }
    
    fore.resize(64,64);
    fore.zero();
    m_tx = back.width()/2;
    m_ty = back.height()*0.4;
    m_tdx = 1;
    m_tdy = 0;
}


void scramble(unsigned char& ch, float f) {
    int x = ch;
    static int idx = 0;
    //x += (int)(Random::normal(0,x*f));
    x += (int)(rnds[idx]*f);
    idx = (idx+17)%MAXRND;
    if (x<0) x = 0;
    if (x>255) x = 255;
    ch = (unsigned char) x;
}


bool AthleteBot::open(yarp::os::Searchable& config) {
    ConstString backFile = config.check("background",Value("textures/back.ppm"),
                                        "background image to use").asString();
    if (backFile!="") {
        yarp::sig::file::read(back,backFile.c_str());
    }
    ConstString foreFile = config.check("target",Value("textures/fore.ppm"),
                                        "target image to use").asString();
    if (foreFile!="") {
        yarp::sig::file::read(fore,foreFile.c_str());
    }
    noiseLevel = config.check("noise",Value(0.05),
                              "pixel noise level").asDouble();
    
    xScale = config.check("sx",Value(1.0),
                          "scaling for x coordinate").asDouble();
    yScale = config.check("sy",Value(1.0),
                          "scaling for y coordinate").asDouble();
    
    lifetime = config.check("lifetime",Value(-1.0),
                            "device should exist for this length of time (in seconds)").asDouble();
    if (lifetime>=0) {
        start();
    }
    return true;
}


// IFrameGrabberImage
bool AthleteBot::getImage(yarp::sig::ImageOf<yarp::sig::PixelRgb>& image) {
    pos[0] += vel[0];
    pos[1] += vel[1];
    double xx = pos[0];
    double yy = pos[1];
    double dx = (xx-m_dx)*5;
    double dy = (yy-m_dy)*5;
    m_tx += m_tdx;
    m_ty += m_tdy;
    if (m_tdx>0 && m_tx>back.width()*0.75) {
        m_tdx *= -1;
    }
    if (m_tdx<0 && m_tx<back.width()*0.25) {
        m_tdx *= -1;
    }
    dx /= 40;
    dy /= 40;
    if (amp[0]>0.5) {
        m_dx += dx;
    }
    if (amp[1]>0.5) {
        m_dy += dy;
    }
    image.resize(m_w,m_h);
    back.safePixel(-1,-1) = PixelRgb(255,0,0);
    loc[0] = m_dx;
    loc[1] = m_dy;
    
    double m_dx_scaled = m_dx*xScale;
    double m_dy_scaled = m_dy*yScale;
    IMGFOR(image,x,y) {
        int x0 = int(x+m_x+m_dx_scaled*0.5+0.5);
        int y0 = int(y+m_y+m_dy_scaled*0.5+0.5);
        image(x,y) = back.safePixel(x0,y0);
        
        if (fore.isPixel(int(x0-m_tx),int(y0-m_ty))) {
            PixelRgb& pix = fore(int(x0-m_tx),int(y0-m_ty));
            if (pix.r<200||pix.g>100||pix.b>100) {
                image(x,y) = pix;
            }
        }
    }
    IMGFOR(image,x2,y2) {
        PixelRgb& pix = image(x2,y2);
        float f = (float)(noiseLevel);
        scramble(pix.r,f);
        scramble(pix.g,f);
        scramble(pix.b,f);
    }
    Time::delay(0.1); // simulated hardware delay
    return true;
}

void AthleteBot::run() {
    if (lifetime>=0) {
        Time::delay(lifetime);
        yarp::os::exit(0);
    }
}

