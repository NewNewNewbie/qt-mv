#ifndef TESTMODEL_H
#define TESTMODEL_H

#include <deque>
#include <set>

#include <QAbstractTableModel>

#include "common.h"

class Type
{
    bool mInitialized;
    Types mType;

protected:

    Type(
            Types iType,
            bool iInitialized = true
            ) :
        mInitialized(iInitialized),
        mType(iType)
    {}

public:

    void initialized(bool iOn){ mInitialized = iOn; }
    bool initialized() const { return mInitialized; }
    constexpr Types type() const { return mType; }
};

class Boolean : public Type
{
    bool mValue;

public:

    Boolean() :
        Type(TypeBool,false),
        mValue(false)
    {

    }

    Boolean(
            bool iInitial
            ) :
        Type(TypeBool),
        mValue(iInitial)
    {

    }

    void setValue(bool iNewValue)
    {
        mValue = iNewValue;
    }

    bool value() const
    {
        return mValue;
    }
};

class Double : public Type
{
    double mValue;

public:

    Double() :
        Type(TypeDouble,false),
        mValue(0)
    {

    }

    Double(
            double iInitial
            ) :
        Type(TypeDouble),
        mValue(iInitial)
    {

    }

    void setValue(double iNewValue)
    {
        mValue = iNewValue;
    }

    double value() const
    {
        return mValue;
    }
};

class String : public Type
{
    QString mValue;

public:

    String() :
        Type(TypeString,false)
    {

    }

    String(
            const QString& iInitial
            ) :
        Type(TypeString),
        mValue(iInitial)
    {

    }

    void setValue(const QString& iValue)
    {
        mValue = iValue;
    }

    const QString& value() const
    {
        return mValue;
    }
};

class Object
{
    using Map = std::map<QString,std::shared_ptr<Type> >;

    const QString mName;
    Map mMap;

public:

    Object(
            const QString& iName
            ) :
        mName(iName)
    {}

    const QString& name() const { return mName; }
    const Map& map() const { return mMap; }

    void emplace(
            const QString& iKey,
            std::unique_ptr<Type>&& iValue
            )
    {
        mMap.try_emplace(
                    iKey,
                    std::move(iValue)
                    );
    }
};

class TestModel : public QAbstractTableModel
{
    Q_OBJECT    

    std::set<QString> mKeys;
    std::deque<std::shared_ptr<Object> > mColumnsHeader;
    std::deque<QString> mRowsHeader;

    std::deque<std::shared_ptr<Object> > mData;

public:

    TestModel(QObject *parent
              ) :
        QAbstractTableModel(parent)
    {

    }

    void addObject(std::shared_ptr<Object>&& iObject)
    {
        for(auto lItem : iObject->map())
        {
            const auto& lKey = lItem.first;

            if(mKeys.emplace(lKey).second)
            {
                mRowsHeader.emplace_back(lKey);
            }
        }

        mColumnsHeader.emplace_back(iObject);

        mData.emplace_back(std::move(iObject));
    }

protected:

    virtual
    Qt::ItemFlags
    flags(
            const QModelIndex &index
            ) const override
    {
        return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
    }

    virtual int rowCount(const QModelIndex &/*parent*/) const override
    {
        return mRowsHeader.size();
    }

    virtual int columnCount(const QModelIndex &parent) const override
    {
        return mColumnsHeader.size();
    }

   virtual
   QVariant
   headerData(
           int section,
           Qt::Orientation orientation,
           int role
           ) const override
   {
        if(role == Qt::DisplayRole)
        {
            if(orientation == Qt::Horizontal)
            {
               //Extracting an object name
                return mColumnsHeader[section]->name();
            }
            else if(orientation == Qt::Vertical)
            {
                //Extracting a key name
                return mRowsHeader[section];
            }
        }

        return QVariant();
   }

    virtual
    QVariant
    data(
            const QModelIndex &iIndex,
            int iRole
            ) const override
    {
        if(iRole == Qt::DisplayRole || iRole == Qt::EditRole)
        {
            auto& lObjectMap = mData[iIndex.column()]->map();

            const auto lIterator = lObjectMap.find(mRowsHeader[iIndex.row()]);

            if(lIterator != lObjectMap.end())
            {
                //Item found
                const auto& lRecord = lIterator->second;

                if(lRecord->initialized())
                {
                    switch(lRecord->type())
                    {
                    case TypeBool :
                        return std::static_pointer_cast<Boolean>(lRecord)->value();
                    case TypeDouble :
                        return QString::number(std::static_pointer_cast<Double>(lRecord)->value(),'f',4);
                    case TypeString :
                        return std::static_pointer_cast<String>(lRecord)->value();
                    }
                }
                else
                {
                    return QString();
                }
            }
            else
            {
                //Item not found
                return QString('-');
            }
        }        
        else if(iRole == TypeRole)
        {
            auto& lObjectMap = mData[iIndex.column()]->map();

            const auto lIterator = lObjectMap.find(mRowsHeader[iIndex.row()]);

            if(lIterator != lObjectMap.end())
            {
                //Item found
                return lIterator->second->type();
            }
        }

        return QVariant();
    }

    virtual
    bool
    setData(
            const QModelIndex &iIndex,
            const QVariant &iValue,
            int iRole
            ) override
    {
        if(iRole == Qt::EditRole)
        {
            auto& lObjectMap = mData[iIndex.column()]->map();

            const auto lIterator = lObjectMap.find(mRowsHeader[iIndex.row()]);

            if(lIterator != lObjectMap.end())
            {
                //Item found
                const auto& lRecord = lIterator->second;

                lRecord->initialized(true);

                switch(lRecord->type())
                {
                case TypeBool :
                    std::static_pointer_cast<Boolean>(lRecord)->setValue(iValue.toBool());
                    break;
                case TypeDouble :
                    std::static_pointer_cast<Double>(lRecord)->setValue(iValue.toDouble());
                    break;
                case TypeString :
                    std::static_pointer_cast<String>(lRecord)->setValue(iValue.toString());
                    break;
                default :
                    return false;
                }

                emit dataChanged(iIndex,iIndex);

                return true;
            }
        }

        return false;
    }
};

#endif // TESTMODEL_H
