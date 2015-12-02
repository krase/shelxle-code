/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include "highlighter.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QCompleter;
class QWidget;
class ChGL;
QT_END_NAMESPACE

class LineNumberArea;
class AfixHighlightArea;

/*! \brief CodeEditor is the editor of shelXle featuring syntax higlighter, code completer, LineNumberArea, AfixHighlightArea and Part Higlighter.
 *
 *
 */
class CodeEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);
    ~CodeEditor();
    int midCursorpos;
    bool ListFile;
    //QSyntaxHighlighter *highlighter;//!< Highlighter is the syntax higlighter. 
    Highlighter *highlighter;//!< Highlighter is the syntax higlighter. 
    void setCompleter(QCompleter *c);//!< setCompleter sets the QCompleter for the editor.
    QCompleter *completer() const;//!< completer is the QCompleter
    QStringList blocks; //!< blocks is a QStringList containing the lines of the editor.
    QList<int> errorInLine; //!< A list of line numbers with syntax errors.

    void lineNumberAreaPaintEvent(QPaintEvent *event);//!< The paint event of LineNumberArea.
    void afixHighlightAreaPaintEvent(QPaintEvent *event);//!< The paint event of AfixHighlightArea.
    int lineNumberAreaWidth();//!< calulates the width of the LineNumberArea.
    void diffTo(QString org);//!< A simple line diff used for the SaveHistoryWidget. Changes back ground color of differering lines into orange. @param org The original file content. 
    QList<int> parenthesis;//!< List of lines.  If the line in an afix environment the value is greater than zero. 
    QList<int> parenthesis2;//!< List of lines. The value is the current part number.
    QList<bool> parenthesis3;//!< List of lines. If it is OK to have nested afixes like in afix 66 it is true.
    QList<bool> comment;//!< List of all lines true if they are comments
    QList<double> fv;//!< List of 'Free Variables' in the SHELX res or ins file. 
    int fvMinLine;//!< fvMinLine specifies the line number of the FVAR istruction. 
signals:
    void findInStructure(const QString &);//!< findInStructure is emitted by a context menu to select and center an atom in the OpenGL representation of the structure.  
    void electInStructure(const QString &);//!< electInStructure is emitted by a context menu to select atoms found in the editor text selection in the OpenGL representation.
    void search(const QString&);//!< searches a QString in the editor. Used to point the cursor to the atom clicked in the OpenGL representation.
    void deleteSelected();//!< deleteSelected is emitted by a context menu to select and delete atoms found in the editor text selection in the OpenGL representation.
protected:
    void resizeEvent(QResizeEvent *event);
    bool viewportEvent(QEvent *event);
    void keyPressEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *e);
    void contextMenuEvent(QContextMenuEvent *event);
public slots:
    void updateLineNumberAreaWidth(int newBlockCount);//!< updateLineNumberAreaWidth sets the new width of LineNumberArea for the given amount of lines newBlockCount. @param newBlockCount number of lines in the editor.
//    void highlightCurrentLine();
//    void highlightSelection(const QString &find);
    void updateLineNumberArea(const QRect &, int);//!< updateLineNumberArea in the curent viewport.
    void updateAfix();//!< updateAfix updates the parenthesis list for the AfixHighlightArea. 
    void insertCompletion(const QString &completion);//!< insertCompletion inserts the text selected in the completer. @param completion selected text of the completer.
    void searchInStructure();//!< Select and center an atom in the OpenGL representation of the structure.
    void selectInStructure();//!< Select atoms found in the editor text selection in the OpenGL representation.
    void lineNumberToggled(QMouseEvent *event);//!< when the LineNumberArea is clicked the comment state of the secified line is toggled.
    void remark(int line);//!< toggles the comment state of the secified line. @param line The line number in file
    void toggleRemarks();//!< toggleRemarks toggles the comment state of the selected lines.
    void setgl(ChGL *g){
      chgl=g;
    }//!< setgl passes the ChGL widget to the CodeEditor

private:
    QString textUnderCursor() const;
    QWidget *lineNumberArea;
    QCompleter *c;
    QWidget *afixHighlightArea;
    ChGL *chgl;
};

/////////////////////////////////////////////////
/*! \brief AfixHighlightArea is a colored bar right of the LineNumberArea to indicate if the line is in a AFIX environment.
 *
 *  This is just a dummy class to reimplement the paintEvent which is impemented in CodeEditor.
 *
 */
class AfixHighlightArea : public QWidget{
public:
  AfixHighlightArea(CodeEditor *editor) : QWidget(editor){
    codeEditor = editor;
  }
  QSize sizeHint() const {
    return QSize(5,0);
  }
protected:
  void paintEvent(QPaintEvent *event){
    codeEditor->afixHighlightAreaPaintEvent(event);
  }//!< calls afixHighlightAreaPaintEvent().

private:
  CodeEditor *codeEditor;
};
/////////////////////////////////////////////////
/*! \brief LineNumberArea is a area with line numbers left of the CodeEditor.
 *
 * This is just a dummy class to reimplement the paintEvent and mousePressEvent which are impemented in CodeEditor.
 */
class LineNumberArea : public QWidget{
public:
  LineNumberArea(CodeEditor *editor) : QWidget(editor) {
    codeEditor = editor;
  }

  QSize sizeHint() const {
    return QSize(codeEditor->lineNumberAreaWidth(), 0);
  }

protected:
  void paintEvent(QPaintEvent *event) {
    codeEditor->lineNumberAreaPaintEvent(event);
  }//!< calls lineNumberAreaPaintEvent().
  void mousePressEvent ( QMouseEvent * event ){
    codeEditor->lineNumberToggled(event);
  }//!< calls lineNumberToggled().

private:
  CodeEditor *codeEditor;
};

#endif
