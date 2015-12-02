
/****************************************************************************
**
** Copyright (C) 2011 Christian B. Huebschle & George M. Sheldrick
** All rights reserved.
** Contact: chuebsch@moliso.de
**
** This file is part of the ShelXle
**
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file COPYING included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**
****************************************************************************/
#include "fourxle.h"
#include <QtCore>
#include <QtGui>
//üäöß
FourXle::FourXle(Molecule *mole_, ChGL *chgl_,QToolBar *toolView, double resol, double wght){
  mole =mole_;
  n1=n2=n3=n4=n5=0;
  chgl=chgl_;
  oldatomsize=-1;
  map_radius=5.0;
  maptrunc = 1;
  chgl->foubas[0]=0;
  chgl->foubas[1]=0;
  chgl->foubas[2]=0;
  HKLMX=200;
  datfo_fc=datfo=NULL;
  nodex=nodey=nodez=NULL;noGoMap=NULL;
  urs=V3(0,0,0);
  nr=0;
  nc=0;
  sigma[0]=sigma[1]=iso[0]=iso[1]=0;
  lintrans=0.8;
  linwidth=0.5;
  rr=resol;
  rw=wght; 
  chgl->foact=toolView->addAction(QIcon(":/icons/fomap.png"),"toggle Fo map",chgl,SLOT(updateGL()));
  connect(chgl->foact,SIGNAL(destroyed(QObject*)),this,SLOT(foactDestroyed()));
  chgl->foact->setCheckable(true);
  chgl->fofcact=toolView->addAction(QIcon(":/icons/diffmap.png"),"toggle Fo-Fc map",chgl,SLOT(updateGL()));
  connect(chgl->fofcact,SIGNAL(destroyed(QObject*)),this,SLOT(fofcactDestroyed()));
  chgl->fofcact->setCheckable(true);
  chgl->fofcact->setVisible(false);
  chgl->foact->setVisible(false);
  doMaps = new QCheckBox("Calculate Maps");
  doMaps->setChecked(true);

  char *ENUS=setlocale(LC_ALL,"C");
  if (!ENUS)ENUS=setlocale(LC_ALL,"en_US.UTF-8"); 
  //printf("%s\n",ENUS);
}

FourXle::~FourXle(){
  killmaps();
  delete doMaps; 
}

void FourXle::killmaps(){
  /*! deletes all fourier maps and frees the memory
   */
  if (chgl->fofcact!=NULL) chgl->fofcact->setVisible(false);
  if (chgl->foact!=NULL) chgl->foact->setVisible(false);
  if (datfo!=NULL) free(datfo);
  if (datfo_fc!=NULL) free(datfo_fc);
  datfo=datfo_fc=NULL;
  deleteLists();
  if (nodex!=NULL) free(nodex);
  if (nodey!=NULL) free(nodey);
  if (nodez!=NULL) free(nodez);
  if (noGoMap!=NULL) free(noGoMap);
  nodex=nodey=nodez=NULL;noGoMap=NULL;
  disconnect(chgl,SIGNAL(diffscroll(int ,int )),0,0);
  disconnect(chgl,SIGNAL(neuemitte(V3)),0,0);
  disconnect(chgl,SIGNAL(inimibas()),0,0);

}

bool FourXle::loadFouAndPerform(const char filename[],bool neu){
  /*! loads a fcf file prepares the reciprocal data for the fourier transpormation and performs it.
   */
  foti.start();

  const int it[143]= {2,3,4,5,6,8,9,10,12,15,16,18,20,24,25,27,30,32,36,40,45,48,50,54,60,64,72,75,80,81,90,96,100,
    108,120,125,128,135,144,150,160,162,180,192,200,216,225,240,243,250,256,270,288,300,320,324,360,375,384,400,405,
    432,450,480,486,500,512,540,576,600,625,640,648,675,720,729,750,768,800,810,864,900,960,972,1000,1024,1080,1125,
    1152,1200,1215,1250,1280,1296,1350,1440,1458,1500,1536,1600,1620,1728,1800,1875,1920,1944,2000,2025,2048,2160,
    2187,2250,2304,2400,2430,2500,2560,2592,2700,2880,2916,3000,3072,3125,3200,3240,3375,3456,3600,3645,3750,3840,
    3888,4000,4050,4096,4320,4374,4500,4608,4800,4860,5000};//!multiples of 2 3 and 5 



  killmaps();
  int ok;
  if (!doMaps->isChecked()) return false;
  if (strstr(filename,".fcf")==NULL) return false;
  FILE *f;
  ok= readHeader(filename);
  if (ok) {
    switch (ok){
    case 1: emit bigmessage("<font color=red>Map generation failed. SHELXL LIST code was not 6.</font>");break;
    case 2: emit bigmessage(QString("<font color=red>Map generation failed. File %1 corrupted.</font>").arg(QString::fromLocal8Bit(filename)));break;
    case 3: emit bigmessage(QString("<font color=red>Map generation failed. Cannot open file %1.</font>").arg(QString::fromLocal8Bit(filename)));break;
    case 4: emit bigmessage(QString("<font color=red>Map generation failed. No reflection data in file %1.</font>").arg(QString::fromLocal8Bit(filename)));break;
    }
    return false;
  }
  f=fopen(filename,"rb");
  if (f==NULL)return false;
  char line[122]="";
  while (strstr(line,"_refln_phase_calc")==NULL) {
    fgets(line,120,f);
  }
  nr=0;
  lr[nr].ih=0;
  lr[nr].ik=0;
  lr[nr].il=0; 
  lr[nr].fo=f000*f000;
  lr[nr].so=0.5f;
  lr[nr].fc=f000;
  lr[nr].phi=0.0f;
  nr=1;
//  printf("f000 %g\n",f000);
  emit bigmessage(QString::fromLocal8Bit(filename));

  while (!feof(f)){
    fgets(line,120,f);
    int rdi=
    sscanf(line,"%d %d %d %f %f %f %f",&lr[nr].ih,&lr[nr].ik, &lr[nr].il ,&lr[nr].fo, &lr[nr].so, &lr[nr].fc, &lr[nr].phi);
    if (rdi==7) {
      if ((abs(lr[nr].ih)<HKLMX)&&
		    (abs(lr[nr].ik)<HKLMX)&&
		    (abs(lr[nr].il)<HKLMX)&&
		    ((lr[nr].ih|lr[nr].ik|lr[nr].il)!=0))
      
        //printf("%4d%4d%4d %g %g\n", lr[nr].ih, lr[nr].ik, lr[nr].il ,lr[nr].fo, lr[nr].so);
       nr++;
    }

   // else printf("?? %d {%s}\n",rdi,line);

    if (nr>=LM) {
        emit bigmessage(QString("<font color=red>to many reflections in fcf file</font>"));
      return false;
    }
  }
  fclose(f);
  if (nr<2) {
      emit bigmessage(QString("<font color=red>Map generation failed. No reflection data in file %1.</font>").arg(QString::fromLocal8Bit(filename)));
      return false;
  }

  for (int i=0;i<nr;i++){
    double u=lr[i].ih,v=lr[i].ik,w=lr[i].il;
    int mh=lr[i].ih,mk=lr[i].ik,ml=lr[i].il;
    double p,q=lr[i].phi/180.0*M_PI;
    lr[i].phi=fmod(4*M_PI+q,2*M_PI);
    for (int k=0; k<ns; k++){
      int nh,nk,nl;
      double t=1.0;
      nh=(int) (u*sy[0][k]+ v*sy[3][k] + w*sy[6][k]);
      nk=(int) (u*sy[1][k]+ v*sy[4][k] + w*sy[7][k]);
      nl=(int) (u*sy[2][k]+ v*sy[5][k] + w*sy[8][k]);
      if((nl<0)||((nl==0)&&(nk<0))||((nl==0)&&(nk==0)&&(nh<0)))
      {nh*=-1;nk*=-1;nl*=-1;t=-1.0;}
      if ((nl<ml)||((nl==ml)&&(nk<mk))||((nl==ml)&&(nk==mk)&&(nh<=mh))) continue;
      mh=nh;mk=nk;ml=nl;
      p=u*sy[9][k]+v*sy[10][k]+w*sy[11][k];
      lr[i].phi=fmod(4*M_PI+t*fmod(q-2*M_PI*p,2*M_PI)-0.01,2*M_PI)+0.01;

    }
    lr[i].ih=mh;
    lr[i].ik=mk;
    lr[i].il=ml;
  }
  sorthkl(nr,lr);
  int n=-1;
  {int i=0;
    while(i<nr){
      double t=0.;
      double u=0.;
      double v=0.;
      double z=0.;
      double p=0.;
      int m;
      int k=i;
      while ((i<nr)&&(lr[i].ih==lr[k].ih)&&(lr[i].ik==lr[k].ik)&&(lr[i].il==lr[k].il)) {
	t=t+1.;
	u+=lr[i].fo;
	v+=1./(lr[i].so*lr[i].so);
	z+=lr[i].fc;
	p=lr[i].phi;
	i++;
      }
      m=n+1;
      lr[m].fo=sqrt(fmax(0.,u/t));
      lr[m].so=sqrt(lr[m].fo*lr[m].fo+sqrt(1./v))-lr[m].fo;
      lr[m].fc=z/t;
      lr[m].phi=p;
      n=m;
      lr[n].ih=lr[k].ih;
      lr[n].ik=lr[k].ik;
      lr[n].il=lr[k].il;
    }
  }
  n++;
  nr=n;
//  printf("%4d%4d%4d %g %g %g %g\n",lr[0].ih,lr[0].ik,lr[0].il,lr[0].fo,lr[0].so,lr[0].fc,lr[0].phi);
  {
    float DX;
    float DY;
    float DZ;	


    {
      int mh=0, mk=0, ml=0,j;
      for (int n=0; n<nr; n++){
	double u=lr[n].ih,v=lr[n].ik,w=lr[n].il;
	double a,b,c;
	for (int k=0; k<ns;k++){
	  a=abs((int)(u*sy[0][k]+v*sy[3][k]+w*sy[6][k]));
	  b=abs((int)(u*sy[1][k]+v*sy[4][k]+w*sy[7][k]));
	  c=abs((int)(u*sy[2][k]+v*sy[5][k]+w*sy[8][k]));
	  mh=(mh<a)?a:mh;
	  mk=(mk<b)?b:mk;
	  ml=(ml<c)?c:ml;
	}
      }
      j=(int)(rr*mh+.5);
      for (int i=0; it[i]< j; i++)n1=it[i+1];
      j=(int)(rr*mk+.5);
      for (int i=0; it[i]< j; i++)n2=it[i+1];
      j=(int)(rr*ml+.5);
      for (int i=0; (it[i]< j)||((nc)&&(it[i]%2)); i++) n3=it[i+1];
      n4=n2*n1;
      n5=n3*n4;
      datfo=(float*) malloc(sizeof(float)*n5);
      datfo_fc=(float*) malloc(sizeof(float)*n5);
      DX=1.0/n1;
      DY=1.0/n2;
      DZ=1.0/n3;
    } 
    for (int typ=0; typ<2;typ++){
      double miZ=99999.99,maZ=-99999.99;
//      mInimum[typ]=99999.99; mAximum[typ]=-99999.99;
      int nbytes,dims[3];
      dims[0]=n3;
      dims[1]=n2;
      dims[2]=n1;
#ifdef FFTW3_H
      B=(fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex)*n5);
      for (int i=0; i<n5; i++){B[i][0]=0;B[i][1]=0;}
#else
      B=(kiss_fft_cpx*)KISS_FFT_MALLOC(nbytes = (sizeof(kiss_fft_cpx)*n5));
      for (int i=0; i<n5; i++){B[i].r=0;B[i].i=0;}
#endif
      for (int i=0; i<nr;i++){
	float  u,v,w;
	u=lr[i].ih;
	v=lr[i].ik;
	w=lr[i].il;
	float  ss,s=0,t=0,q,p;
	for (int n=0; n<ns;n++){
	  int j,k,l;
	  j=(int) (u*sy[0][n]+ v*sy[3][n] + w*sy[6][n]);
	  k=(int) (u*sy[1][n]+ v*sy[4][n] + w*sy[7][n]);
	  l=(int) (u*sy[2][n]+ v*sy[5][n] + w*sy[8][n]);
	  if((abs(j-lr[i].ih)+abs(k-lr[i].ik)+abs(l-lr[i].il))==0)s+=1.0;
	  if(abs(j+lr[i].ih)+abs(k+lr[i].ik)+abs(l+lr[i].il)==0)t+=1.0;
	}
        if(typ==0) ss=(lr[i].fo-lr[i].fc)/(C[14]*(s+t));
	else if(typ==2) ss=(lr[i].fc)/(C[14]*(s+t));
        else ss=(lr[i].fo)/(C[14]*(s+t));
	if(lr[i].fc>1.E-6) ss=ss/(1.+rw*pow(lr[i].so/lr[i].fc,4));
	for (int n=0; n<ns;n++){
	  int j,k,l,m;
	  j=(int) (u*sy[0][n]+ v*sy[3][n] + w*sy[6][n]);
	  k=(int) (u*sy[1][n]+ v*sy[4][n] + w*sy[7][n]);
	  l=(int) (u*sy[2][n]+ v*sy[5][n] + w*sy[8][n]);
//          q=(-2*M_PI*(u*sy[9][n]+v*sy[10][n]+w*sy[11][n]))-M_PI*(j*DX+k*DY+l*DZ);
          q=(lr[i].phi-2*M_PI*(u*sy[9][n]+v*sy[10][n]+w*sy[11][n]))-M_PI*(j*DX+k*DY+l*DZ);
	  j=(999*n1+j)%n1;
	  k=(999*n2+k)%n2;
	  l=(999*n3+l)%n3;
	  m=j+n1*(k+n2*l);
	  p=ss*cosf(q);
	  q=ss*sinf(q);
#ifdef FFTW3_H
	  B[m][0]=p;
	  B[m][1]=q;
#else
	  B[m].r=p;
	  B[m].i=q;
#endif
	  j*=-1;
	  if(j<0)j=n1+j;
	  k*=-1;
	  if(k<0)k=n2+k;
	  l*=-1;
	  if(l<0)l=n3+l;
	  m=j+n1*(k+n2*l);
#ifdef FFTW3_H
	  B[m][0]=p;
	  B[m][1]=-q;
#else
	  B[m].r=p;
	  B[m].i=-q;
#endif
	}
      }
#ifdef FFTW3_H
      fwd_plan = fftwf_plan_dft_3d(n3,n2,n1,B,B,FFTW_FORWARD,FFTW_ESTIMATE);
      fftwf_execute(fwd_plan);
      fftwf_destroy_plan(fwd_plan);
#else
      fwd_plan = kiss_fftnd_alloc(dims,3,0,0,0);
      kiss_fftnd( fwd_plan,B,B);
      free(fwd_plan);
#endif
      float t=0;
      double DM=0.,  DS=0., DD  ;
      for (int i=0; i<n5;i++){
#ifdef FFTW3_H
	DD=B[i][0];
#else
	DD=B[i].r;
#endif
//	maxi[typ]=fmax(maxi[typ],DD); mini[typ]=fmin(mini[typ],DD);
	miZ=fmin(miZ,DD);
	maZ=fmax(maZ,DD);
	DM+=DD;
	DS+=DD*DD;
#ifdef FFTW3_H
        if (typ==1) datfo[i]=B[i][0];
        else if (typ==0) datfo_fc[i]=B[i][0];
#else
        if (typ==1) datfo[i]=B[i].r;
        else if (typ==0) datfo_fc[i]=B[i].r;

#endif
      }
      sigma[typ]=t=sqrt((DS/n5)-((DM/n5)*(DM/n5)));

/*      if (typ==1){
      double r=9999.0/(maZ-miZ);
      double s=1/r;
      FILE *map=fopen("test.map","wb");
      fprintf(map,"%5d%5d%5d%5d%12.8f%12.6f%12.6f\n",1,n1,n2,n3,s,miZ,t);
      for (int mzi=0; mzi<n5; mzi++){
          fprintf(map,"%4d ",(int)( (B[mzi].r-miZ)*r));
          if (mzi%16==15)fprintf(map,"\n");
      }
      fclose(map);
      }*/
      free(B);
    }//1
  }//2
  urs=V3(0,0,0);int gt=0;
  for (int i=0; i<mole->showatoms.size();i++)
    if (mole->showatoms.at(i).an>-1) {urs+=mole->showatoms.at(i).frac;gt++;}
  urs*=1.0/gt;
  urs=V3(1,1,1)-1.0*urs;
  mole->frac2kart(urs,urs);

  nodex= (FNode*)malloc(sizeof(FNode)*n5);
  nodey= (FNode*)malloc(sizeof(FNode)*n5);
  nodez= (FNode*)malloc(sizeof(FNode)*n5);
  noGoMap= (int*) malloc(sizeof(int)*n5*27);
  for (int no=0;no<(n5*27);no++) noGoMap[no]=0; 
  for (int o=0; o<n5;o++){
    nodex[o].flag=0;
    nodey[o].flag=0;
    nodez[o].flag=0;
  }
  dx=V3(1.0/(n1),0,0);
  dy=V3(0,1.0/(n2),0);
  dz=V3(0,0,1.0/(n3));
  mole->frac2kart(dx,dx); 
  mole->frac2kart(dy,dy); 
  mole->frac2kart(dz,dz);     
  delDA[ 0]=  -n1*dx -n2*dy -n3*dz;//nx ny,nz??
  delDA[ 1]=         -n2*dy -n3*dz;
  delDA[ 2]=   n1*dx -n2*dy -n3*dz;
  delDA[ 3]=  -n1*dx        -n3*dz;
  delDA[ 4]=                -n3*dz;
  delDA[ 5]=   n1*dx        -n3*dz;
  delDA[ 6]=  -n1*dx +n2*dy -n3*dz;
  delDA[ 7]=          n2*dy -n3*dz;
  delDA[ 8]=   n1*dx +n2*dy -n3*dz;
  delDA[ 9]=  -n1*dx -n2*dy       ;
  delDA[10]=         -n2*dy       ;
  delDA[11]=   n1*dx -n2*dy       ;
  delDA[12]=  -n1*dx              ;
  delDA[13]=  V3(0,0,0)           ;
  delDA[14]=   n1*dx              ;
  delDA[15]=  -n1*dx +n2*dy       ;
  delDA[16]=          n2*dy       ;
  delDA[17]=   n1*dx +n2*dy       ;
  delDA[18]=  -n1*dx -n2*dy +n3*dz;
  delDA[19]=         -n2*dy +n3*dz;
  delDA[20]=   n1*dx -n2*dy +n3*dz;
  delDA[21]=  -n1*dx        +n3*dz;
  delDA[22]=                +n3*dz;
  delDA[23]=   n1*dx        +n3*dz;
  delDA[24]=  -n1*dx +n2*dy +n3*dz;
  delDA[25]=          n2*dy +n3*dz;
  delDA[26]=   n1*dx +n2*dy +n3*dz;
  balken->setMinimum(0);
  balken->setMaximum(n3*6);
  balken->show();
  balkenstep=0;
  emit bigmessage(QString("Uniq Reflections: %1 <br>Fourier grid dimensions: %2 %3 %4 = %5 grid points.<br>Time for loading and fourier transformation: <b>%6 s</b>.<br>").arg(nr).arg(n1).arg(n2).arg(n3).arg(n5).arg(foti.elapsed()/1000.0,1,'f',1));
  if (neu) gen_surface(neu);
  balken->hide();
  return true;
}

void FourXle::deleteLists(){
  /*! deletes the display lists of the fourier maps.
   */
 for (int fac=0; fac<18; fac++){
    if ((chgl->foubas[fac])&&(glIsList(chgl->foubas[fac]))) {
      //printf("deleting list %d %d %d\n", chgl->foubas[fac], glIsList(chgl->foubas[fac]),fac);
      glDeleteLists(chgl->foubas[fac],1);
      chgl->foubas[fac]=0;
    }
 }
}

void FourXle::trimm(char s[]){
  /*! a trimm function for c-strings.
   */
  char sc[409];
  int j=0;
  int len=strlen(s);
  strncpy(sc,s,400);
  for (int i=0; i<len; i++) if ((sc[i]!='\'')&&(!isspace(sc[i]))) s[j++]=toupper(sc[i]);
  s[j]='\0';
}

void FourXle::deletes(char *s, int count){
  /*! deletes count characters at the begining of s.
   */
  if ((s==NULL)||(count <1)||((size_t)count>strlen(s))) return;
  for (int i=0; i<count; i++) s[i]=' ';
  trimm(s);
}

int FourXle::readHeader(const char *filename){
  /*! reads the header of an fcf file
   */
  FILE *f=NULL;
  char line[122],*dum=NULL;
  //size_t zlen=120;
  int ok=0;
  int i;double T,V;
  f=fopen(filename,"r");
  if (f==NULL) return 3;
  ns=0;
  sy[0][ns]=1.0;
  sy[1][ns]=0.0;
  sy[2][ns]=0.0;

  sy[3][ns]=0.0;
  sy[4][ns]=1.0;
  sy[5][ns]=0.0;

  sy[6][ns]=0.0;
  sy[7][ns]=0.0;
  sy[8][ns]=1.0;

  sy[9][ns]=0.0;
  sy[10][ns]=0.0;
  sy[11][ns]=0.0;
  ns=1;
  int listcode=0;
  do{
    dum=fgets(line,120,f);
    if (dum==NULL){fclose(f);return 2;};
    if (feof(f)){fclose(f);return 2;};
    while (dum[0]==' ') dum++;
    if (!strncmp(dum,"_shelx_title",12)) {
      sscanf(line,"_shelx_title %[^\r\n]",titl);
      trimm(titl);
    }
    if (!strncmp(dum,"_exptl_crystal_F_000",20)){
        sscanf(line,"_exptl_crystal_F_000 %f",&f000);
    }
    if (!strncmp(dum,"_shelx_refln_list_code",22)) {
        sscanf(line,"_shelx_refln_list_code %d",&listcode);
        //qDebug()<<listcode;
        if (listcode!=6) {fclose(f);return 1;}
    }
    if (!strncmp(dum,"_cell_length_a",14)) {
      sscanf(line,"_cell_length_a %lf",&C[0]);
    } 
    if (!strncmp(dum,"_cell_length_b",14)) {
      sscanf(line,"_cell_length_b %lf",&C[1]);
    } 
    if (!strncmp(dum,"_cell_length_c",14)) {
      sscanf(line,"_cell_length_c %lf",&C[2]);
    } 
    if (!strncmp(dum,"_cell_angle_alpha",17)) {
      sscanf(line,"_cell_angle_alpha %lf",&C[3]);
    } 
    if (!strncmp(dum,"_cell_angle_beta",16)) {
      sscanf(line,"_cell_angle_beta %lf",&C[4]);
    } 
    if (!strncmp(dum,"_cell_angle_gamma",17)) {
      sscanf(line,"_cell_angle_gamma %lf",&C[5]);
      for (i=0;i<3;i++){
        if (C[i]<0.1) return 2;
	T=.0174533*C[i+3];
        if (T<0.001) return 2;
	D[i]=sin(T);
	D[i+3]=cos(T);
	C[i+6]=(D[i]/(C[i]*C[i]));
      } 
      V=1.-D[3]*D[3]-D[4]*D[4]-D[5]*D[5]+2.*D[3]*D[4]*D[5];
      C[6]/=V;
      C[7]/=V;
      C[8]/=V;
      C[9]= 2.*sqrt(C[7]*C[8])*(D[4]*D[5]-D[3])/(D[2]*D[2]);
      C[10]=2.*sqrt(C[6]*C[8])*(D[3]*D[5]-D[4])/(D[0]*D[2]);
      C[11]=2.*sqrt(C[6]*C[7])*(D[3]*D[4]-D[5])/(D[0]*D[1]);
      C[14]=C[1]*C[2]*C[0]*sqrt(V);
      D[6]=C[1]*C[2]*D[0]/C[14];
      D[7]=C[0]*C[2]*D[1]/C[14];
      D[8]=C[0]*C[1]*D[2]/C[14];
    }
    if ((!strncmp(dum,"_symmetry_equiv_pos_as_xyz",26))||(!strncmp(dum,"_space_group_symop_operation_xyz",32))){
//      char s1[50],s2[50],s3[50];
//      char *kill=NULL,*nom=NULL,*div=NULL ;
      dum=fgets(line,120,f);
      trimm(line);
      while (strchr(line,'Y')) {
	QString sc=QString(line).toUpper().remove("SYMM").trimmed();
	sc=sc.remove("'");
	sc=sc.remove(" ");
	QStringList axe=sc.split(",");
	QStringList bruch;
	if (axe.size()!=3) return false;
	double _sx[3],_sy[3],_sz[3],t[3];
	for (int i=0; i<3; i++){
	  _sx[i]=0;_sy[i]=0;_sz[i]=0;t[i]=0;
	  if (axe.at(i).contains("-X")) {_sx[i]=-1.0;axe[i].remove("-X");}
	  else if (axe.at(i).contains("X")) {_sx[i]=1.0;axe[i].remove("X");}
	  if (axe.at(i).contains("-Y")) {_sy[i]=-1.0;axe[i].remove("-Y");}
	  else if (axe.at(i).contains("Y")) {_sy[i]=1.0;axe[i].remove("Y");}
	  if (axe.at(i).contains("-Z")) {_sz[i]=-1.0;axe[i].remove("-Z");}
	  else if (axe.at(i).contains("Z")) {_sz[i]=1.0;axe[i].remove("Z");}
	  if (axe.at(i).endsWith("+")) axe[i].remove("+");
	  if (axe.at(i).contains("/")) {
	    bruch=axe.at(i).split("/");
	    if (bruch.size()==2) t[i]=bruch.at(0).toDouble() / bruch.at(1).toDouble();
	  }
	  else if (!axe.at(i).isEmpty()) t[i]=axe.at(i).toDouble();
	}
        sy[0][ns]=_sx[0];
        sy[1][ns]=_sy[0];
        sy[2][ns]=_sz[0];

        sy[3][ns]=_sx[1];
        sy[4][ns]=_sy[1];
        sy[5][ns]=_sz[1];

        sy[6][ns]=_sx[2];
        sy[7][ns]=_sy[2];
        sy[8][ns]=_sz[2];


        sy[9][ns]=t[0];
        sy[10][ns]=t[1];
        sy[11][ns]=t[2];


	strcpy(line,"");
	dum=fgets(line,120,f);
	trimm(line);
	ns++;

      }
    }
    if (!strncmp(dum,"_refln_phase_calc",17)) ok=1;
  }while((!ok)&&(!feof(f)));

  if (listcode!=6) return 1;
  for (int i=0; i<ns;i++){
    for (int n=i+1; n<ns;n++){
    int u=0,v=0;
    for (int j=0; j<9; j++){
      u+=abs(sy[j][n]-sy[j][i]);
      v+=abs(sy[j][n]+sy[j][i]);
    }
    if (fmin(u,v)>0.01) continue;
    for (int j=0; j<12; j++){
      sy[j][n]=sy[j][ns-1];
    }
    ns--;
    }
  }
  fclose(f);

  return 0;
}

void FourXle::sorthkl(int nr, Rec r[]){
  /*! sorts the reflection list
   */
  Rec *hilf= (Rec*) malloc(sizeof(Rec)*nr);
  int i,j,k,nj,ni,spalte;int index[4096];
  for (spalte=0; spalte<3; spalte++){
    j=-999999;
    k=999999;
    switch (spalte) {
	    case 0: for (i=0; i<nr; i++){ j=(j<r[i].ih)?r[i].ih:j; k=(k>r[i].ih)?r[i].ih:k; } break;
	    case 1: for (i=0; i<nr; i++){ j=(j<r[i].ik)?r[i].ik:j; k=(k>r[i].ik)?r[i].ik:k; } break;
	    case 2: for (i=0; i<nr; i++){ j=(j<r[i].il)?r[i].il:j; k=(k>r[i].il)?r[i].il:k; } break;
    }
    nj=-k;
    ni=(nj+j+1);
    for (i=0; i<=ni; i++) index[i]=0;
    for (i=0; i<nr; i++){
      switch (spalte){
	      case 0: j=r[i].ih+nj; break;
	      case 1: j=r[i].ik+nj; break;
	      case 2: j=r[i].il+nj; break;
      }
      index[j]++;/*brauch ich das? -->JA!*/
      hilf[i].ih=r[i].ih;
      hilf[i].ik=r[i].ik;
      hilf[i].il=r[i].il;
      hilf[i].fo=r[i].fo;
      hilf[i].so=r[i].so;
      hilf[i].fc=r[i].fc;
      hilf[i].phi=r[i].phi;
    }/*/4*/
   j=0;
      for (i=0; i<ni; i++){
	k=j;
	j+=index[i];
	index[i]=k;     
      }/*/5*/
   for (i=0; i<nr;i++){
     switch (spalte) {
	     case 0: j=hilf[i].ih +nj;break;
	     case 1: j=hilf[i].ik +nj;break;
	     case 2: j=hilf[i].il +nj;break;
     }     
     index[j]++;   
     j=index[j]-1;
     r[j].ih=hilf[i].ih;
     r[j].ik=hilf[i].ik;
     r[j].il=hilf[i].il;
     r[j].fo=hilf[i].fo;
     r[j].so=hilf[i].so;
     r[j].fc=hilf[i].fc;
     r[j].phi=hilf[i].phi;
   }/*/6*/
  }/*/spalten*/
free(hilf);
}

void FourXle::bewegt(V3 nm){/*!moves the rotation center to nm*/
  V3 v , alturs=urs;
  mole->kart2frac(nm,v);
  urs=V3(1,1,1)-1.0*v;
  mole->frac2kart(urs,urs);
  if ((chgl->objCnt==acnt)&&(maptrunc==2)) return; 
  if (urs==alturs) return;
  balken->setMinimum(0);
  balken->setMaximum(n3*6);
  balken->show();
  balkenstep=0;
  gen_surface(false);
  balken->hide();
  chgl->pause=false;
  chgl->updateGL();
}

void FourXle::inimap(){/*! reinitializes the display lists for screenshots*/
  //for screenies
  balken->setMinimum(0);
  balken->setMaximum(n3*6);
  balken->show();
  balkenstep=0;
  deleteLists();
  gen_surface(false);
  balken->hide();
}
#ifndef _MSC_VER
#include <omp.h>
#endif

void FourXle::gen_surface(bool neu,int imin,int imax){
  if (noGoMap==NULL) return;
  /*! creates iso surfaces for fo-fc- fo-fc+ fo+ and fo- maps if neu then the iso values are calculated fro the sigma value of the map.
   *
   */
  if (!chgl->neutrons) imax=qMin(3,imax);
    V3 v=V3(0,0,0);
    mole->kart2frac(chgl->altemitte,v);
    urs=V3(1,1,1)-1.0*v;
    mole->frac2kart(urs,urs);

  suti.start();
  disconnect(chgl,SIGNAL(diffscroll(int ,int )),0,0);
  disconnect(chgl,SIGNAL(neuemitte(V3)),0,0);
  disconnect(chgl,SIGNAL(inimibas()),0,0);
//  if ((neu)||(mole->showatoms.size()!=oldatomsize)||(chgl->unhide->isVisible())){
  for (int no=0;no<(n5*27);no++) noGoMap[no]=0; 
    mole->kart2frac(dx,dxc);
    mole->kart2frac(dy,dyc);
    mole->kart2frac(dz,dzc);
    int incx = (int) (1.4/dx.x);
    int incy = (int) (1.4/dy.y);
    int incz = (int) (1.4/dz.z);
    int incmin=qMin(incx,qMin(incy,incz));
    incmin*=incmin;
  for (int g=0; g<mole->showatoms.size();g++){
    if (mole->showatoms.at(g).hidden) continue;
    mole->kart2frac(mole->showatoms.at(g).pos,oc);
    int ax=(int)((oc.x)/dxc.x-0.499), bx=(int)((oc.y)/dyc.y-0.499), cx=(int)((oc.z)/dzc.z-0.499);
    for (int aa=ax-incx; aa<ax+incx; aa++){
      for (int bb=bx-incy; bb<bx+incy; bb++){
	for (int cc=cx-incz; cc<cx+incz; cc++){
	  if (incmin<((aa-ax)*(aa-ax)+(bb-bx)*(bb-bx)+(cc-cx)*(cc-cx))) continue;
	  int dEx=dex3(aa,bb,cc);
	  if ((dEx>0)&&(dEx<27*n5)) 
	    noGoMap[dEx]=1;
	}
      }
    }
  }
//  oldatomsize=mole->showatoms.size();
//  }
tri=0;
 for (int fac=imin; fac<imax; fac++){
    if ((chgl->foubas[fac])&&(glIsList(chgl->foubas[fac]))) glDeleteLists(chgl->foubas[fac],1);

    switch (fac){
	    case 0:                   
                    if (neu) iso[1]=sigma[0]*2.7;
                    else iso[1]=fabs(iso[1]);
                    mtyp=1;
		    break;
            case 1:
                    if (neu) iso[1]=-sigma[0]*2.7;
                    else iso[1]=-fabs(iso[1]);
                    mtyp=1;
		    break;
	    case 2:                   
                   if (neu) iso[0]=sigma[1]*1.2;
		   //printf("blau %g %d\n",iso[0],neu);
                    mtyp=0;
		    break;
	    case 3:                   
                   if (neu) iso[0]=-sigma[1]*1.2;
                    else iso[0]=-fabs(iso[0]);
		   //printf("orange %g %d\n",iso[0],neu);
                    mtyp=0;
		    break;
    }
    int chunk=20;
    balkenstep++;
    if (balken->value()*1.1<balkenstep) balken->setValue(balkenstep);
#pragma omp parallel shared(chunk)
    {
      int ix,iy,iz;
#pragma omp for schedule(dynamic,chunk) private(ix,iy) nowait
      for (iz=0; iz<n3;iz++){
	for (iy=0; iy<n2;iy++){
	  for (ix=0; ix<n1;ix++){
	    CalcVertex(ix,iy,iz);
	  }
	}
      }
    }//omp
    balken->setValue(balkenstep+=n3);
    if (chgl->lighting->isChecked())    {

#pragma omp parallel shared(chunk)
      {
	int ix,iy,iz;
#pragma omp for schedule(dynamic,chunk) private(ix,iy) nowait
	for(iz=0; iz<n3; iz++ ){
	  for(iy=0; iy<n2; iy++ ){
	    for(ix=0; ix<n1; ix++ ){
	      int kk=dex(ix,iy,iz);
	      if( nodex[kk] ){
		nodex[kk].normal = CalcNormalX(ix,iy,iz);
	      }
	      if( nodey[kk] ){
		nodey[kk].normal = CalcNormalY(ix,iy,iz);
	      }
	      if( nodez[kk] ){
		nodez[kk].normal = CalcNormalZ(ix,iy,iz);
	      }
	    }
	  }
	}
      }//#pragma omp parallel
    }
    int mper=(chgl->niceTrans->isChecked())?6:1;
    for (int pers=0;pers<mper;pers++){
      if (!chgl->foubas[fac+pers*4]) chgl->foubas[fac+pers*4]=glGenLists(1);
      glNewList(chgl->foubas[fac+pers*4], GL_COMPILE );
      glLineWidth(linwidth);
	switch (fac) {
		case 0: 
			dipc.setAlphaF(lintrans);
			chgl->qglColor(dipc);
//			glColor4d(0.0, 0.7, 0.0, lintrans);
			break;
		case 1: 
			dimc.setAlphaF(lintrans);
			chgl->qglColor(dimc);
//			glColor4d(0.8, 0.0, 0.0, lintrans);
			break;
		case 2: 
			fopc.setAlphaF(lintrans);
			chgl->qglColor(fopc);
//			glColor4d(0.0, 0.0, 1.0, lintrans);
			break;
		case 3: 
			fomc.setAlphaF(lintrans);
			chgl->qglColor(fomc);
//			glColor4d(1.0, 0.65, 0.0, lintrans);
			break;
	}
	glPushMatrix();
	glScaled(chgl->L,chgl->L,chgl->L);
	int h,k,l;
	//#pragma omp for private(h,k,l)
	int step=0;
	switch (pers) {
		case 0:
			for ( h=0; h<n1;h++){
			  for ( k=0; k<n2;k++){
			    for ( l=0; l<n3;l++){
			      MakeElement(h,k,l,n1,n4);
			    }
			  }
			  step++;step%=mper;
			  if (step==0) balken->setValue(balkenstep++);
			}
			break;
		case 1:
			for ( h=n1; h>=0;h--){
			  for ( k=n2; k>=0;k--){
			    for ( l=n3; l>=0;l--){
			      MakeElement(h,k,l,n1,n4);
			    }
			  }
			  step++;step%=mper;
			  if (step==0) balken->setValue(balkenstep++);
			}
			break;
		case 2:
			for ( k=0; k<n2;k++){
			  for ( h=0; h<n1;h++){
			    for ( l=0; l<n3;l++){
			      MakeElement(h,k,l,n1,n4);
			    }
			  }
			  step++;step%=mper;
			  if (step==0) balken->setValue(balkenstep++);
			}
			break;
		case 3:
			for ( k=n2; k>=0;k--){
			  for ( h=n1; h>=0;h--){
			    for ( l=n3; l>=0;l--){
			      MakeElement(h,k,l,n1,n4);
			    }
			  }
			  step++;step%=mper;
			  if (step==0) balken->setValue(balkenstep++);
			}
			break;
		case 4:
			for ( l=0; l<n3;l++){
			  for ( h=0; h<n1;h++){
			    for ( k=0; k<n2;k++){
			      MakeElement(h,k,l,n1,n4);
			    }
			  }
			  step++;step%=mper;
			  if (step==0) balken->setValue(balkenstep++);
			}
			break;
		case 5:
			for ( l=n3; l>=0;l--){
			  for ( h=n1; h>=0;h--){
			    for ( k=n2; k>=0;k--){
			      MakeElement(h,k,l,n1,n4);
			    }
			  }
			  step++;step%=mper;
			  if (step==0) balken->setValue(balkenstep++);
			}
			break;
	}
	glPopMatrix();
      glEndList();
    }
  }
 iso[0]=fabs(iso[0]);

  QString info=QString("<b>Fo-Fc1 Map:</b><font color=green>%1 e&Aring;<sup>-3</sup></font>"
		  "<font color=red> %2 e&Aring;<sup>-3</sup> (&sigma; = %7) </font><br><font color=grey> Hint:  [%3 Scroll (up or down)] to change. </font><br>"
                  "<b>Fo-Map:</b><font color=blue>%4  e&Aring;<sup>-3</sup> (&sigma; = %8)</font><br><font color=grey> Hint:  [%5 Scroll (up or down)] to change. </font><br>Time for creating map surfaces <b>%6 s</b>. %9 Triangles drawn.<br>")
	  .arg(-iso[1],6,'g',2)
	  .arg(iso[1],6,'g',2)
	  .arg(QKeySequence(Qt::ControlModifier).toString(QKeySequence::NativeText))
	  .arg(iso[0],6,'g',2)
	  .arg(QKeySequence(Qt::ShiftModifier).toString(QKeySequence::NativeText))
	  .arg(suti.elapsed()/1000.0,1,'f',1)
	  .arg(sigma[0],6,'g',2)
	  .arg(sigma[1],6,'g',2)
	  .arg(tri)
	  ;
  emit bigmessage(info);
  connect(chgl,SIGNAL(inimibas()),this,SLOT(inimap()));
  connect(chgl,SIGNAL(neuemitte(V3)),this, SLOT(bewegt(V3)));
  connect(chgl,SIGNAL(diffscroll(int ,int )),this,SLOT(change_iso(int ,int )));
  chgl->fofcact->setVisible(((chgl->foubas[0])&&(glIsList(chgl->foubas[0]))));
  chgl->foact->setVisible(((chgl->foubas[2])&&(glIsList(chgl->foubas[2]))));

  acnt=chgl->objCnt;
}

void FourXle::change_iso(int numsteps,int diff){
  /*! canges the iso value of the fo or fo-fc maps and redraws them
   */
  disconnect(chgl,SIGNAL(diffscroll(int ,int )),0,0);
  iso[diff]=fabs(iso[diff]);
  iso[diff]+=iso[diff]*numsteps/10.0;
  int mi=0,ma=5;
  balken->setMinimum(0);
  balken->setMaximum(n3*(2+diff*2));
  balken->show();
  balkenstep=0;
  switch (diff) {
	  case 0: 
		  mi=2;ma=4;
		  break;
	  case 1:
		  mi=0; ma=2;
		  break;
  
  }
  gen_surface(false,mi,ma);
  balken->hide();
  chgl->updateGL();
}

void FourXle::foactDestroyed() {
  chgl->foact = NULL;
}

void FourXle::fofcactDestroyed() {
  chgl->fofcact = NULL;
}

void FourXle::CalcVertex( int ix, int iy, int iz) {
  V3 mdz=(0.5*dx)+(0.5*dy)+(0.5*dz);
  V3 o,fl,m2u=V3(0.5,0.5,0.5);
  mole->frac2kart(m2u,m2u);
  double vo=0, vx=0,vy=0,vz=0;
  int idx=dex(ix,iy,iz);
  nodex[idx].flag=0;
  nodey[idx].flag=0;
  nodez[idx].flag=0;
  if (mtyp==0){//*datfo,*datfo_fc,*datf1_f2
    vo = datfo[ idx]   -iso[mtyp];
    vx = datfo[ dex(ix+1,iy,iz)]   -iso[mtyp];
    vy = datfo[ dex(ix,iy+1,iz)]   -iso[mtyp];
    vz = datfo[ dex(ix,iy,iz+1)]   -iso[mtyp];
  }else {
    vo = datfo_fc[idx]   -iso[mtyp];
    vx = datfo_fc[dex(ix+1,iy,iz)]   -iso[mtyp];
    vy = datfo_fc[dex(ix,iy+1,iz)]   -iso[mtyp];
    vz = datfo_fc[dex(ix,iy,iz+1)]   -iso[mtyp];
}
  V3 nor=V3(0,0,0);//Normalize((vx-vo)*dx+(vy-vo)*dy+(vz-vo)*dz);
  if (Intersect(vo,vx)) {
  if (maptrunc==2) o=dx*((vo/(vo-vx))+ix) + dy*iy + dz*iz+m2u;
  else  o=dx*((vo/(vo-vx))+ix) + dy*iy + dz*iz+urs;
  
    mole->kart2frac(o,o);
    o+=V3(-0.5,-0.5,-0.5);
    fl=V3(floor(o.x),floor(o.y),floor(o.z));
    o+=-1.0*fl;
    o+=V3(0.5,0.5,0.5);
    mole->frac2kart(o,o);
    if (maptrunc!=2) o+=-1.0*urs;
    else o+=-1.0*m2u;
    o+=mdz;
//    orte.append(o);
    nodex[idx].vertex=o;
    nodex[idx].normal=nor;
    nodex[idx].flag=1;
  }
  if (Intersect(vo,vy)) {
    if (maptrunc==2)o=dx*ix + dy*((vo/(vo-vy))+iy) + dz*iz+m2u;
    else o=dx*ix + dy*((vo/(vo-vy))+iy) + dz*iz+urs;
    mole->kart2frac(o,o);
    o+=V3(-0.5,-0.5,-0.5);
    fl=V3(floor(o.x),floor(o.y),floor(o.z));
    o+=-1.0*fl;
    o+=V3(0.5,0.5,0.5);
    mole->frac2kart(o,o);
    if (maptrunc!=2) o+=-1.0*urs;
    else o+=-1.0*m2u;
    o+=mdz;
//    orte.append(o);
    nodey[idx].vertex=o;
    nodey[idx].normal=nor;
    nodey[idx].flag=1;
  }
  if (Intersect(vo,vz)) {
    if (maptrunc==2)o=dx*ix + dy*iy + dz*((vo/(vo-vz))+iz)+m2u;
    else o=dx*ix + dy*iy + dz*((vo/(vo-vz))+iz)+urs;
    mole->kart2frac(o,o);
    o+=V3(-0.5,-0.5,-0.5);
    fl=V3(floor(o.x),floor(o.y),floor(o.z));
    o+=-1.0*fl;
    o+=V3(0.5,0.5,0.5);
    mole->frac2kart(o,o);
    if (maptrunc!=2) o+=-1.0*urs;
    else o+=-1.0*m2u;
    o+=mdz;
//    orte.append(o);
    nodez[idx].vertex=o;
    nodez[idx].normal=nor;
    nodez[idx].flag=1;
  }
}

V3& FourXle::VectorSelected( FNode& node0, FNode& node1, FNode& node2, FNode& node3 ) {
  if( node1 && node2 && node3 ){
    GLfloat d1 = Distance( node0.vertex, node1.vertex ) +
            Distance( node3.vertex, node2.vertex );
    GLfloat d2 = Distance( node0.vertex, node2.vertex ) +
            Distance( node3.vertex, node1.vertex );
    if( d1 > d2 ) return node2.vertex; else return node1.vertex;
  }else{
    if(      node1 )   return node1.vertex;
    else if( node2 )   return node2.vertex;
    else if( node3 )   return node3.vertex;
  }
  return node0.vertex;
}

void FourXle::makeFaces(int n, FNode poly[] ){
  int sign[12];
  if (n<3) return;  //weniger als 3 verts -> nichts zu tun
  V3 mid_ver = V3(0,0,0);
  V3 mid_nor = V3(0,0,0);
  //V3 mid_nor2 = V3(0,0,0);
  for(int i=0; i<n; i++ ){
    mid_ver += poly[i].vertex;
    if (!chgl->lighting->isChecked()) continue;
    if (Norm(mid_nor+poly[i].normal)>=Norm(mid_nor-poly[i].normal)) {
      mid_nor += poly[i].normal;
      sign[i]=1;
    }
    else {
      mid_nor += -1.0*poly[i].normal;
      sign[i]=-1;
    }
  }
  mid_ver *= (1.0/n);
  if (chgl->lighting->isChecked()) mid_nor= Normalize(mid_nor);
  //mid_nor2= Normalize(mid_nor2);
  V3 mit=V3(1,1,1);
  mole->frac2kart(mit,mit);
  for (int w=0; w<27; w++){
    if (maptrunc==0){w=13;}
    else if ((maptrunc==1)&&(Distance(mit-urs,mid_ver+delDA[w])>(map_radius*map_radius))) continue;
    else if (maptrunc==2) {
      mole->kart2frac(mid_ver+delDA[w],oc);
      int ax=(int)((oc.x)/dxc.x-0.499), bx=(int)((oc.y)/dyc.y-0.499), cx=(int)((oc.z)/dzc.z-0.499);
      int DEX=dex3(ax,bx,cx);
      if (!noGoMap[DEX]) continue;
    }

    {
    tri+=n;
    glBegin(GL_TRIANGLE_FAN);
    if (chgl->lighting->isChecked()) {glNormal3d( mid_nor.x,mid_nor.y,mid_nor.z);}
    glVertex3d( mid_ver.x+delDA[w].x,mid_ver.y+delDA[w].y,mid_ver.z+delDA[w].z);
    for (int k=0; k<=n;k++){
      if (chgl->lighting->isChecked()) {glNormal3d( sign[k%n]*poly[k%n].normal.x, sign[k%n]*poly[k%n].normal.y, sign[k%n]*poly[k%n].normal.z);}
      glVertex3d(     poly[k%n].vertex.x+delDA[w].x,
		      poly[k%n].vertex.y+delDA[w].y,
		      poly[k%n].vertex.z+delDA[w].z);
    }
    glEnd();
  }
  if (maptrunc==0) w=27;
  }
}//omp

int FourXle::IndexSelected( FNode& node0, FNode& node1, FNode& node2, FNode& node3 ) {
  if( node1 && node2 && node3 ){
    GLfloat d1 = Distance( node0.vertex, node1.vertex) +
            Distance( node3.vertex, node2.vertex) ;
    GLfloat d2 = Distance( node0.vertex, node2.vertex ) +
            Distance( node3.vertex, node1.vertex );
    if( d1 > d2 ) return 2; else return 1;
  }else{
    if(      node1 )   return 1;
    else if( node2 )   return 2;
    else if( node3 )   return 3;
  }
  return 0;

}

void FourXle::MakeElement( int ix, int iy, int iz ,int s1, int s2) {
  int conn[12][2][4] = {
    {{ 0, 1, 7, 6}, { 0, 2, 8, 3}},  //  0
    {{ 1, 2, 5, 4}, { 1, 0, 6, 7}},  //  1
    {{ 2, 0, 3, 8}, { 2, 1, 4, 5}},  //  2
    {{ 3, 8, 2, 0}, { 3, 4,10, 9}},  //  3
    {{ 4, 3, 9,10}, { 4, 5, 2, 1}},  //  4
    {{ 5, 4, 1, 2}, { 5, 6, 9,11}},  //  5
    {{ 6, 5,11, 9}, { 6, 7, 1, 0}},  //  6
    {{ 7, 6, 0, 1}, { 7, 8,11,10}},  //  7
    {{ 8, 7,10,11}, { 8, 3, 0, 2}},  //  8
    {{ 9,10, 4, 3}, { 9,11, 5, 6}},  //  9
    {{10,11, 8, 7}, {10, 9, 3, 4}},  // 10
    {{11, 9, 6, 5}, {11,10, 7, 8}}   // 11
  };
  FNode node[12];
  FNode polygon[12];
  node[ 0] = nodex[(ix+iy*s1+iz*s2)        %n5];        // 000x
  node[ 1] = nodey[(ix+iy*s1+iz*s2)        %n5];        // 000y
  node[ 2] = nodez[(ix+iy*s1+iz*s2)        %n5];        // 000z
  node[ 3] = nodex[(ix+iy*s1+((iz+1)%n3)*s2)    %n5];    // 001y
  node[ 4] = nodey[(ix+iy*s1+((iz+1)%n3)*s2)    %n5];    // 001z
  node[ 5] = nodez[(ix+((iy+1)%n2)*s1+iz*s2)    %n5];    // 010x
  node[ 6] = nodex[(ix+((iy+1)%n2)*s1+iz*s2)    %n5];    // 010y
  node[ 7] = nodey[(((1+ix)%n1)+iy*s1+iz*s2)      %n5];      // 100y
  node[ 8] = nodez[(((1+ix)%n1)+iy*s1+iz*s2)      %n5];      // 100z
  node[ 9] = nodex[(ix+((iy+1)%n2)*s1+((iz+1)%n3)*s2)%n5];// 011x
  node[10] = nodey[(((ix+1)%n1)+iy*s1+((iz+1)%n3)*s2)%n5];// 101y
  node[11] = nodez[(((ix+1)%n1)+((iy+1)%n2)*s1+iz*s2)%n5];// 110z
  if (((char)node[0]+node[1]+node[2]+node[3]+node[4]+node[5]
                          +node[6]+node[7]+node[8]+node[9]+node[10]+node[11])==0) return;
  for( int is=0; is<12; is++ ) {
    if( !node[is] ) continue;

    int n=0, i=is, m=0;//,ai=i;
    GLfloat dis;
    dis=0;
    do {
      polygon[n++]= node[i];
      int sol = IndexSelected(
                      node[conn[i][m][0]],
                      node[conn[i][m][1]],
                      node[conn[i][m][2]],
                      node[conn[i][m][3]]);
      //ai=i;
      i = conn[i][m][sol];
      if( sol == 2 ) m ^= 1;
      dis+=Distance(polygon[0].vertex,node[i].vertex);
        node[i].flag= 0;
    }while( (i!=is)&&(n<11) );
    if (n>=3) {
      if (dis<5)
      makeFaces( n, polygon );
      else {
        int axe=0;
        double delx=0,dely=0,delz=0;
        double mind=100000000;
        V3 minp=V3(10000,10000,10000),lihiun=V3(-10000,-10000,-10000);
        int minii=0;
        for (int polni=1; polni<=n; polni++){
            delx+=fabs(polygon[polni-1].vertex.x - polygon[polni%n].vertex.x);
            dely+=fabs(polygon[polni-1].vertex.y - polygon[polni%n].vertex.y);
            delz+=fabs(polygon[polni-1].vertex.z - polygon[polni%n].vertex.z);
            if (Distance(polygon[polni%n].vertex,lihiun)<mind) {
              mind=Distance(polygon[polni%n].vertex,minp);
              minii=polni%n;
            }
        }
        minp=polygon[minii].vertex;
        axe|=(delx>1)?1:0;
        axe|=(dely>1)?2:0;
        axe|=(delz>1)?4:0;
        for (int polni=0; polni<n; polni++){
          V3 neo=polygon[polni].vertex;
          double lang =Distance(minp,neo);
          if ((lang>Distance(minp,neo+dx*n1))&&(axe&1)) neo+=dx*n1;
          else if ((lang>Distance(minp,neo-dx*n1))&&(axe&1)) neo+=-n1*dx;
          lang =Distance(minp,neo);
          if ((lang>Distance(minp,neo+dy*n2))&&(axe&2)) neo+=n2*dy;
          else if ((lang>Distance(minp,neo-dy*n2))&&(axe&2)) neo+=-n2*dy;
          lang =Distance(minp,neo);
          if ((lang>Distance(minp,neo+n3*dz))&&(axe&4)) neo+=n3*dz;
          else if ((lang>Distance(minp,neo-n3*dz))&&(axe&4)) neo+=-n3*dz;
          polygon[polni].vertex=neo;
        }
        dis=0;
        for (int polni=1; polni<=n; polni++){
            dis+=Distance(polygon[polni-1].vertex , polygon[polni%n].vertex);
        }
      if (dis<5)
        makeFaces( n, polygon );
      }
    }
  }

}

V3 FourXle::CalcNormalX( int ix, int iy, int iz ) {
  V3 tang[4];
    tang[0] = VectorSelected(nodex[dex(ix,iy,iz)],nodey[dex(ix,iy,iz)],
                             nodey[dex(ix+1,iy,iz)],nodex[dex(ix,iy+1,iz)]);
    tang[1] = VectorSelected(nodex[dex(ix,iy,iz)],nodey[dex(ix,iy-1,iz)],
                             nodey[dex(ix+1,iy-1,iz)],nodex[dex(ix,iy-1,iz)]);
    tang[2] = VectorSelected(nodex[dex(ix,iy,iz)],nodez[dex(ix,iy,iz)],
                             nodez[dex(ix+1,iy,iz)],nodex[dex(ix,iy,iz+1)]);
    tang[3] = VectorSelected(nodex[dex(ix,iy,iz)],nodez[dex(ix,iy,iz-1)],
                             nodez[dex(ix+1,iy,iz-1)],nodex[dex(ix,iy,iz-1)]);
    return Normalize((tang[0] - tang[1])%(tang[2] - tang[3]));
}

V3 FourXle::CalcNormalY( int ix, int iy, int iz ) {
  V3 tang[4];
    tang[0] = VectorSelected(nodey[dex(ix,iy,iz)],nodex[dex(ix,iy,iz)],
                             nodex[dex(ix,iy+1,iz)],nodey[dex(ix+1,iy,iz)]);
    tang[1] = VectorSelected(nodey[dex(ix,iy,iz)],nodex[dex(ix-1,iy,iz)],
                             nodex[dex(ix-1,iy+1,iz)],nodey[dex(ix-1,iy,iz)]);
    tang[2] = VectorSelected(nodey[dex(ix,iy,iz)],nodez[dex(ix,iy,iz)],
                             nodez[dex(ix,iy+1,iz)],nodey[dex(ix,iy,iz+1)]);
    tang[3] = VectorSelected(nodey[dex(ix,iy,iz)],nodez[dex(ix,iy,iz-1)],
                             nodez[dex(ix,iy+1,iz-1)],nodey[dex(ix,iy,iz-1)]);
    return Normalize((tang[2] - tang[3])%(tang[0] - tang[1]));
 }

V3 FourXle::CalcNormalZ( int ix, int iy, int iz ) {
  V3 tang[4];
  tang[0] = VectorSelected(nodez[dex(ix,iy,iz)],nodex[dex(ix,iy,iz)],
                           nodex[dex(ix,iy,iz+1)],nodez[dex(ix+1,iy,iz)]);
  tang[1] = VectorSelected(nodez[dex(ix,iy,iz)],nodex[dex(ix-1,iy,iz)],
                           nodex[dex(ix-1,iy,iz+1)],nodez[dex(ix-1,iy,iz)]);
  tang[2] = VectorSelected(nodez[dex(ix,iy,iz)],nodey[dex(ix,iy,iz)],
                           nodey[dex(ix,iy,iz+1)],nodez[dex(ix,iy+1,iz)]);
  tang[3] = VectorSelected(nodez[dex(ix,iy,iz)],nodey[dex(ix,iy-1,iz)],
                           nodey[dex(ix,iy-1,iz+1)],nodez[dex(ix,iy-1,iz)]);
  return Normalize((tang[0] - tang[1])%(tang[2] - tang[3]));
}

void FourXle::jnk(){
  if(!n5)return;
  float mini=9e37,maxi=-9e37;
  float f,r,fstep;
  double DM,DS,sigma,w,e_net,e_gross;
  r=powf((3*(n1-1)*(n2-1)*(n3-1)),1.0f/3.0f);
//  printf("%d %d %d %d %f\n",n1,n2,n3,n5,r);
  float *datfo_fcstp=(float*) malloc(sizeof(float)*n5);
  float invstep=100.0f;
  float step=1.0f/invstep;
  float rhomind2=99999.0f,rhomaxd2=-99999.0f;
  QMap<float,int> hash;
  QMap<float,float> hashf;
  DM=0.0f;
  DS=0.0f;
  w=0.0f;
  for (int i=0; i<n5; i++){
      f=datfo_fc[i];
      mini=qMin(f,mini);
      maxi=qMax(f,maxi);
      DM+=f;
      DS+=f*f;
      w+=fabs(f);
      datfo_fcstp[i]=floorf(f*invstep);
  }// */
  e_net=(DM/n5)*C[14];//C[14] ist das Volumen der UC
  e_gross=w/(2*n5)*C[14];
  sigma=sqrt((DS/n5)-((DM/n5)*(DM/n5)));
  //printf("%g %g %g",e_net,e_gross,sigma);
  for (int zi=0;zi<n3;zi++)
    for (int yi=0;yi<n2;yi++)
      for (int xi=0;xi<(n1-1);xi++){
        if (datfo_fcstp[dex(xi,yi,zi)]>datfo_fcstp[dex(xi+1,yi,zi)]){
          fstep=datfo_fcstp[dex(xi,yi,zi)];
          int ze=fstep-datfo_fcstp[dex(xi+1,yi,zi)];
          for (int zii=0; zii<ze; zii++) hash[(fstep-zii)*step]++;
        }else if (datfo_fcstp[dex(xi,yi,zi)]<datfo_fcstp[dex(xi+1,yi,zi)]){
          fstep=datfo_fcstp[dex(xi+1,yi,zi)];
          int ze=fstep-datfo_fcstp[dex(xi,yi,zi)];
          for (int zii=0; zii<ze; zii++) hash[(fstep-zii)*step]++;
        }
      }
  for (int xi=0;xi<n1;xi++)
    for (int zi=0;zi<n3;zi++)
      for (int yi=0;yi<(n2-1);yi++){
        if (datfo_fcstp[dex(xi,yi,zi)]>datfo_fcstp[dex(xi,yi+1,zi)]){
          fstep=datfo_fcstp[dex(xi,yi,zi)];
          int ze=fstep-datfo_fcstp[dex(xi,yi+1,zi)];
          for (int zii=0; zii<ze; zii++) hash[(fstep-zii)*step]++;
        }else if (datfo_fcstp[dex(xi,yi,zi)]<datfo_fcstp[dex(xi,yi+1,zi)]){
          fstep=datfo_fcstp[dex(xi,yi+1,zi)];
          int ze=fstep-datfo_fcstp[dex(xi,yi,zi)];
          for (int zii=0; zii<ze; zii++) hash[(fstep-zii)*step]++;
        }
      }

  for (int yi=0;yi<n2;yi++)
    for (int xi=0;xi<n1;xi++)
      for (int zi=0;zi<(n3-1);zi++){
        if (datfo_fcstp[dex(xi,yi,zi)]>datfo_fcstp[dex(xi,yi,zi+1)]){
          fstep=datfo_fcstp[dex(xi,yi,zi)];
          int ze=fstep-datfo_fcstp[dex(xi,yi,zi+1)];
          for (int zii=0; zii<ze; zii++) hash[(fstep-zii)*step]++;
        }else if (datfo_fcstp[dex(xi,yi,zi)]<datfo_fcstp[dex(xi,yi,zi+1)]){
          fstep=datfo_fcstp[dex(xi,yi,zi+1)];
          int ze=fstep-datfo_fcstp[dex(xi,yi,zi)];
          for (int zii=0; zii<ze; zii++) hash[(fstep-zii)*step]++;
        }
      }
  free(datfo_fcstp);
  //float range=fmaxf(fabsf(mini),fabsf(maxi));
  QDialog *jnkdlg = new QDialog();
   QGraphicsScene*scene= new QGraphicsScene(-30,-50,550,586);
  scene->setBackgroundBrush(QBrush(QColor("#e9f7d6")));
  scene->clear ();
  QGraphicsItem *itm;
  for (int i=0; i<21;i++){
    itm=scene->addLine(i*25,0,i*25,500,(i%5)?QPen(QColor("#cbdbbb"),0):QPen(QColor("#959d9d"),0));
    itm->setData(0,-1);
  }
  for (int i=0; i<31;i++){
  itm=scene->addLine(0,i*16.66666666666667,500,i*16.66666666666667,(i%5)?QPen(QColor("#cbdbbb"),0):QPen(QColor("#959d9d"),0));
  itm->setData(0,-1);
  }
  itm=scene->addLine(250,0,250,500,QPen(QColor("#000000"),0));
  itm=scene->addLine(0,83.33333333333333,500,83.33333333333333,QPen(QColor("#000000"),0));
  itm=scene->addLine(0,250,500,250,QPen(QColor("#000000"),0));
  itm=scene->addLine(0,416.6666666666667,500,416.6666666666667,QPen(QColor("#000000"),0));
  QGraphicsTextItem *txt = scene->addText("3");
  txt->moveBy(-15,73.33333333333333);
  txt = scene->addText("2");
  txt->moveBy(-15,240);
  txt = scene->addText("1");
  txt->moveBy(-15,406.6666666666667);
  txt = scene->addText("-1.0");
  txt->moveBy(-15,500);
  txt = scene->addText("0");
  txt->moveBy(241,500);
  txt = scene->addText("1.0");
  txt->moveBy(485,500);


  QMapIterator<float, int> i(hash);
  while (i.hasNext()) {
    i.next();
    f=hashf[i.key()]=logf(i.value())/logf(r);
    scene->addEllipse(250+(i.key())*250,500-((f-0.5)/3.)*500,4,4,QPen(Qt::NoPen),QBrush(QColor("#0907e6")));

    if (f>2){
        rhomind2=qMin(rhomind2,i.key());
        rhomaxd2=qMax(rhomaxd2,i.key());
    }
   // printf("jnk: %g %g %g %g %g %g\n",i.key(),f,rhomind2,rhomaxd2,250+(i.key())*250,500-((f-0.5)/3.)*500);
  }
 // printf("\ndf(0)= %g %d %g %g\n",hashf.value(0.0f),hash.value(0.0f),rhomind2,rhomaxd2);

  float m,b;
  m=(hashf.value(rhomind2)-hashf.value(rhomind2-step))/step;
  b=hashf.value(rhomind2)-m*rhomind2;
  rhomind2=(2.0f-b)/m;
  m=(hashf.value(rhomaxd2)-hashf.value(rhomaxd2-step))/step;
  b=hashf.value(rhomaxd2)-m*(rhomaxd2);
  rhomaxd2=(2.0f-b)/m;
 // printf("df(0)= %g %d %g %g\n",hashf.value(0.0f),hash.value(0.0f),rhomind2,rhomaxd2);
  txt = scene->addText(QString("df(0) = %1").arg((double)hashf.value(0.0f),7,'f',2),QFont("Courier",10));
  txt->moveBy(328,2);
  txt = scene->addText(QString("min(d=2) = %1 eA^-3").arg((double)rhomind2,7,'f',3),QFont("Courier",10));
  txt->moveBy(305,18);
  txt = scene->addText(QString("max(d=2) = %1 eA^-3").arg((double)rhomaxd2,7,'f',3),QFont("Courier",10));
  txt->moveBy(305,34);
  txt = scene->addText(QString("min      = %1 eA^-3").arg((double)mini,7,'f',3),QFont("Courier",10));
  txt->moveBy(305,50);
  txt = scene->addText(QString("max      = %1 eA^-3").arg((double)maxi,7,'f',3),QFont("Courier",10));
  txt->moveBy(305,66);
  txt = scene->addText(QString("e_net    = %1 e").arg((double)e_net,8,'g',3),QFont("Courier",10));
  txt->moveBy(295,82);
  txt = scene->addText(QString("e_gross  = %1 e").arg((double)e_gross,8,'g',3),QFont("Courier",10));
  txt->moveBy(295,98);
  txt = scene->addText(QString("         = %1 eA^-3").arg((double)sigma,8,'f',3),QFont("Courier",10));
  txt->moveBy(295,116);
  txt = scene->addText(QString("nx ny nz  %1 x %2 x %3 = %4").arg(n1).arg(n2).arg(n3).arg(n5),QFont("Courier",9));
  txt->moveBy(30,2);
  txt = scene->addText(QString("fractal dimension vs. residual density"),QFont("Helvetica",14,QFont::Bold));
  txt->moveBy(100,-40);
  txt = scene->addText(QString("df"),QFont("Helvetica",12,QFont::Bold));
  txt->moveBy(-20,20);
  txt = scene->addText(QString("0"),QFont("Helvetica",12,QFont::Bold));
  txt->moveBy(460,500);

  #ifdef __APPLE__
    txt = scene->addText(QString::fromUtf8("𝛒"),QFont("Helvetica",16,QFont::Bold));
    txt->moveBy(450,490);
    txt = scene->addText(QString::fromUtf8("𝛒"),QFont("Helvetica",14));
    txt->moveBy(295,8);
    txt = scene->addText(QString::fromUtf8("𝛒"),QFont("Helvetica",14));
    txt->moveBy(295,24);
    txt = scene->addText(QString::fromUtf8("𝛒"),QFont("Helvetica",14));
    txt->moveBy(295,40);
    txt = scene->addText(QString::fromUtf8("𝛒"),QFont("Helvetica",14));
    txt->moveBy(295,56);
    txt = scene->addText(QString::fromUtf8("𝛔"),QFont("Helvetica",14));
    txt->moveBy(295,106);
  #else
  txt = scene->addText(QString::fromUtf8("ρ"),QFont("Helvetica",16,QFont::Bold));
  txt->moveBy(450,490);
  txt = scene->addText(QString::fromUtf8("ρ"),QFont("Helvetica",14));
  txt->moveBy(295,8);
  txt = scene->addText(QString::fromUtf8("ϱ"),QFont("Helvetica",14));
  txt->moveBy(295,24);
  txt = scene->addText(QString::fromUtf8("ϱ"),QFont("Helvetica",14));
  txt->moveBy(295,40);
  txt = scene->addText(QString::fromUtf8("ϱ"),QFont("Helvetica",14));
  txt->moveBy(295,56);
  txt = scene->addText(QString::fromUtf8("σ"),QFont("Helvetica",14));
  txt->moveBy(295,106);
  #endif  // */
  txt = scene->addText(QString("Please cite as: 'K. Meindl, J. Henn, Acta Cryst., 2008, A64, 404-418.'"),QFont("Helvetica",9));
  txt->moveBy(100,516);
  QGraphicsView *view = new QGraphicsView(scene,jnkdlg);

  QVBoxLayout *lt = new QVBoxLayout(jnkdlg);
  lt->addWidget(view);
  jnkdlg->show();


}

inline int FourXle::dex(int x,int y, int z){
  /*! dex is used to adress elemennts of a one dimensional array by three indizes like it is a 3 dimensional array
   * @param x,y,z tree dimensional indices 
   * if x is < 0 or > n1 it is not a problem because % is used to clamp it.
   * if y is < 0 or > n2 it is not a problem because % is used to clamp it.
   * if z is < 0 or > n3 it is not a problem because % is used to clamp it.
   * \returns index of an 1 dimensional array 
   */
    x=(x+n1)%n1;
    y=(y+n2)%n2;
    z=(z+n3)%n3;
   return x+n1*(y+n2*z);
}

inline int FourXle::dex3(int x,int y, int z){
  int n31=3*n1,n32=3*n2,n33=3*n3;
    x=(x+n31)%n31;
    y=(y+n32)%n32;
    z=(z+n33)%n33;
   return x+n31*(y+n32*z);
}
