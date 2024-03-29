
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
#include "window.h"
#include <QApplication>
//öß
void mh(QtMsgType type, const char *msg){
  if (strstr(msg,"QAccessibleWidget")!=NULL) {
    printf("%s\n",msg);fflush(stdout);
    return; // ignore useless error messages on win with touch screens
  }
  switch (type){
  case QtDebugMsg:

    printf("%s\n",msg);fflush(stdout);

#if defined Q_WS_WIN  ||  defined Q_WS_MAC
    QMessageBox::information(0,"information",msg);
#endif
    break;
  case QtWarningMsg:
    printf("%s\n",msg);fflush(stdout);
#if defined Q_WS_WIN  ||  defined Q_WS_MAC
    QMessageBox::warning(0,"warning",msg);
#endif
    break;
  case QtCriticalMsg:
    printf("%s\n",msg);fflush(stdout);
#if defined Q_WS_WIN  || defined Q_WS_MAC
    QMessageBox::critical(0,"critical",msg);
#endif
    break;
  case QtFatalMsg:
    printf("%s\n",msg);fflush(stdout);
#if defined Q_WS_WIN  || defined Q_WS_MAC
    QMessageBox::critical(0,"fatal",msg);
#endif
    abort();
  }
}


int main(int argv, char *args[]){
  QApplication app(argv, args);
  app.setOrganizationName("shelxle");
//  app.setOrganizationDomain("shelxle");
  qInstallMsgHandler(mh);
  app.setWindowIcon(QIcon(":/icons/SHELxLE.png"));
  Window *window = new Window();
  window->show();
  int istat = app.exec();
  delete window;
  return istat;
}
