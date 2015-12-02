#ifndef DRAGDROPATOMSDLG_H
#define DRAGDROPATOMSDLG_H

#include <QtGui>
#include "molecule.h"
/*! \brief DragAtom is an atom label that can be draged in the 'Inherit Labels' feature. 
 *
 * DragAtom is colored like the element the label refers to and represent the old label of the target molecule that 
 * should inhereit its labels from an refence molecule with final labels. It behaves like a fridge magnet from the QtDemo examples.
 */   
class DragAtom  : public QLabel {
public:
        DragAtom(const int index,const QString &text, QColor c=Qt::lightGray, QWidget *parent=0);
        QString labelText() const;//!< The Label of the DragAtom.
        int index() const;//!< The current position index in the list of DragAtoms.
        QColor color() const;//!< The Color of the DragAtom

private:
        QString m_labelText;
        int m_index;
        QColor m_c;
};

/*! \brief DragDropAtomsDialog A Dialog for the 'Inherit Labels' feature. 
 *
 * Features two columns of Atom Labels and DragAtoms. The left colomn referes to the final labeled recerence molecule and the
 * right one referes to the target molecule which shall be labeled the same way. The DragAtoms of the right column can be draged around the list to 
 * change the ordereing sequence if the program was not able to assign them correct automatically.
 */ 
class DragDropAtomsDialog : public QDialog {
Q_OBJECT
public:
    DragDropAtomsDialog(Molecule &mole, const QList<int> &reference, QList<int> *target, QWidget *parent = 0, Qt::WindowFlags f = 0);
    QLineEdit *resiResiClass;//!< A QLineEdit for the residue class which can be 4 letters long or 3 letters to be compatible to the pdb standard.
    QSpinBox *resiNrSpin;//!< A QSpinBox
    QComboBox *resiFinder;//!< A QComboBox that contains all existing residues found in the sructure.
    QPushButton * ok;//!< The Ok Button
signals:
         void pairs(const QString &);//!< When an atom label of the left column is clicked the signal is emitted to select the two atoms in the structure. @param _t1 the atom labels separated by a space. 
public slots:
         void grossRC(const QString &text);//!< The text passed by this slot is capitalized an passed to the resiResiClass text. @param text Text to be capitalized and passed to resiResiClass.setText()
         void renameRNchanged(int ii);//!< called when the resiNrSpin changes the residue number if it is found in the structure the resiResiClass text gets updated and dissabled for user input. @params ii Residue number 
};

/*! \brief DragDropAtoms contains a list of DragAtoms to assign the labels of the reference molecule to the target molecule. 
 */
class DragDropAtoms : public QWidget {
Q_OBJECT
public:
         DragDropAtoms(Molecule &mole, const QList<int> &reference, QList<int> *target, QWidget *parent = 0);

signals:
         void pairs(const QString &);//!< When an atom label of the left column is clicked the signal is emitted to select the two atoms in the structure. @param _t1 the atom labels separated by a space. 
protected:
	 void dragEnterEvent(QDragEnterEvent *event);//!< When draging of an DragAtom starts...
	 void dragMoveEvent(QDragMoveEvent *event);//!< When the DragAtom is moved by the mouse...
	 void dropEvent(QDropEvent *event);//!< When when the DragAtom is released
	 void mousePressEvent(QMouseEvent *event);
        // void mouseMoveEvent(QMouseEvent *event);
 private:
         int itemheight,current;
         QStringList dadas;
	 Molecule m_mole;
         QList<int> *m_target;
         QList<int> m_reference;
         QList<DragAtom *> dragAtoms;
 };
#endif
