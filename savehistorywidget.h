#ifndef SAVEHISTORYWIDGET_h
#define SAVEHISTORYWIDGET_H

#include <QtGui>
#include "molecule.h"
#include "itsme.h"
#include "codeeditor.h"

//! DragVersion is a QLabel that represents an earlier version of a res file and shows the time elapsed since it was saved. Used in SaveHistoryWidget
class DragVersion  : public QLabel {
public:
        DragVersion(const int index,const QString &text, uint secs,const int cur, QWidget *parent=0);
	
        QString labelText() const;//!< \returns the DragVersion text.
        int index() const;//!< \returns positional index
        uint secs() const;//!< \returns seconds elapsed since it was created
//protected:
//	void paintEvent ( QPaintEvent * event ) ;

private:
        QString m_labelText;
        int m_index;
        uint m_secs;
};

class SaveHistoryWidget;

//! DragDropVersions is a widget that contains DragVersion s it is used in the SaveHistoryWidget.
class DragDropVersions : public QWidget {
Q_OBJECT
public:
         DragDropVersions(QString filename, QWidget *parent = 0);
	 QString seit(int wann);
	 QString seitkurzem(int wann);
         QDateTime now;//!< now is the the current QDateTime.
	 QTimer *tick;//!< tick is a QTime creating ticks every second.
         friend class SaveHistoryWidget;
signals:
	 void indexChanged(int index);//!< When a DragVersion is clicked the version viewed changes 
public slots:
	void tack();//!< called every second by tick to change the displayed time on each DragVersion
protected:
//	 void dragEnterEvent(QDragEnterEvent *event);
//	 void dragMoveEvent(QDragMoveEvent *event);
//	 void dropEvent(QDropEvent *event);
	 void mousePressEvent(QMouseEvent *event);
        // void mouseMoveEvent(QMouseEvent *event);
	 void paintEvent ( QPaintEvent * event ) ;
 private:
	 int itemheight,itemwidth,current,breite;
         QList<DragVersion *> dragVersions;
	 QStringList entries,headers;
	 QList<QDateTime> zeiten;
 };
//! SaveHistoryWidget is a QDialog to load and compare earlier versions of the res file. 
class SaveHistoryWidget : public QDialog {
Q_OBJECT
public:
    SaveHistoryWidget(QString filename, QWidget *parent = 0, Qt::WindowFlags f = 0);
    QPushButton * ok;//!> the ok button 
//signals:
public slots:
	void tock();
	void updatePreview(int index);
	void accept();  
  private:
 int pvindex;
    CodeEditor *edi;
    QLabel *pvheader;
    DragDropVersions *dda;
    QString fileName,orginal;
};
#endif

