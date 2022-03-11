#ifndef ITEM_DELEGATE_H
#define ITEM_DELEGATE_H

#include <QAbstractItemDelegate>
#include <QItemDelegate>

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>

#include "common.h"

class ItemDelegate : public QItemDelegate
{
public:

    ItemDelegate(
            QObject* iParent
            ) :
        QItemDelegate(iParent)
    {

    }

private:

    virtual
    QWidget*
    createEditor(
            QWidget *parent,
            const QStyleOptionViewItem &option,
            const QModelIndex &iIndex
            ) const
    {
        const auto& lData = iIndex.data(TypeRole);

        if(lData.isValid())
        {
            switch(lData.toUInt())
            {
            case TypeBool :
            {
                //Create the combobox editor
                QComboBox *editor = new QComboBox(parent);
                editor->addItem("false");
                editor->addItem("true");
                return editor;
            }
            case TypeDouble :
            {
                //Create double spinbox editor
                QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
                editor->setDecimals(4);
                editor->setSingleStep(0.01);
                editor->setMinimum(0);
                editor->setMaximum(100);
                return editor;
            }
            case TypeString :
                //Create line edit
                return new QLineEdit(parent);
            }
        }

        return nullptr;
    }

    virtual
    void
    setEditorData(
            QWidget *iEditor,
            const QModelIndex &iIndex
            ) const
    {
        const auto& lData = iIndex.data(TypeRole);

        if(lData.isValid())
        {
            switch(lData.toUInt())
            {
            case TypeBool :
            {                
                QComboBox *lControl = qobject_cast<QComboBox*>(iEditor);
                //Extract data from model
                const bool lValue = iIndex.model()->data(iIndex, Qt::EditRole).toBool();
                lControl->setCurrentIndex(lValue?(1):(0));
            }
                break;            
            case TypeDouble :
            {                
                QDoubleSpinBox *lControl = qobject_cast<QDoubleSpinBox*>(iEditor);
                //Extract data from model
                const double lValue = iIndex.model()->data(iIndex, Qt::EditRole).toDouble();
                lControl->setValue(lValue);
            }
                break;            
            case TypeString :
            {
                QLineEdit *lControl = static_cast<QLineEdit*>(iEditor);
                //Extract data from model
                QString value = iIndex.model()->data(iIndex, Qt::EditRole).toString();
                lControl->setText(value);
            }
                break;
            }
        }
    }

    virtual
    void
    setModelData(
            QWidget *iEditor,
            QAbstractItemModel *iModel,
            const QModelIndex &iIndex
            ) const
    {
        const auto& lData = iIndex.data(TypeRole);

        if(lData.isValid())
        {
            switch(lData.toUInt())
            {
            case TypeBool :
            {
                //Copy data from editor to model
                QComboBox *lControl = static_cast<QComboBox*>(iEditor);
                int lValue = lControl->currentIndex();
                if(lValue != -1)
                {
                    iModel->setData(iIndex, lValue?(true):(false), Qt::EditRole);
                }
            }
                break;
            case TypeDouble :
            {
                QDoubleSpinBox *lControl = static_cast<QDoubleSpinBox*>(iEditor);
                lControl->interpretText();
                const double lValue = lControl->value();
                iModel->setData(iIndex, lValue, Qt::EditRole);
            }
                break;
            case TypeString :
            {
                QLineEdit *lineEdit = static_cast<QLineEdit*>(iEditor);
                const QString& lValue = lineEdit->text();
                iModel->setData(iIndex, lValue, Qt::EditRole);
            }
                break;
            }
        }
    }

    virtual
    void
    updateEditorGeometry(
            QWidget *editor,
            const QStyleOptionViewItem &option,
            const QModelIndex &/* index */
            ) const
    {
        editor->setGeometry(option.rect);
    }
};

#endif // ITEM_DELEGATE_H
