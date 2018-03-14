/****************************************************************************
** Meta object code from reading C++ file 'general-tab.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "general-tab.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'general-tab.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GeneralTab[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      36,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   32,   32,   32, 0x05,
      33,   32,   32,   32, 0x05,
      59,   32,   32,   32, 0x05,
      77,   32,   32,   32, 0x05,

 // slots: signature, parameters, type, tag, flags
      92,   32,   32,   32, 0x08,
     110,   32,   32,   32, 0x08,
     129,   32,   32,   32, 0x08,
     152,   32,   32,   32, 0x08,
     176,   32,   32,   32, 0x08,
     197,   32,   32,   32, 0x08,
     213,   32,   32,   32, 0x08,
     233,   32,   32,   32, 0x08,
     253,   32,   32,   32, 0x08,
     275,   32,   32,   32, 0x08,
     299,   32,   32,   32, 0x08,
     319,   32,   32,   32, 0x08,
     341,   32,   32,   32, 0x08,
     363,   32,   32,   32, 0x08,
     387,   32,   32,   32, 0x08,
     407,   32,   32,   32, 0x08,
     424,   32,   32,   32, 0x08,
     449,   32,   32,   32, 0x08,
     470,   32,   32,   32, 0x08,
     490,   32,   32,   32, 0x08,
     511,   32,   32,   32, 0x08,
     533,   32,   32,   32, 0x08,
     559,   32,   32,   32, 0x08,
     584,   32,   32,   32, 0x08,
     607,   32,   32,   32, 0x08,
     627,   32,   32,   32, 0x08,
     652,   32,   32,   32, 0x08,
     666,   32,   32,   32, 0x08,
     683,   32,   32,   32, 0x08,
     706,   32,   32,   32, 0x08,
     727,   32,   32,   32, 0x08,
     748,   32,   32,   32, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GeneralTab[] = {
    "GeneralTab\0audioDeviceChanged()\0\0"
    "pixelAspectRatioChanged()\0croppingChanged()\0"
    "clearBuffers()\0inputChanged(int)\0"
    "outputChanged(int)\0inputAudioChanged(int)\0"
    "outputAudioChanged(int)\0standardChanged(int)\0"
    "qryStdClicked()\0timingsChanged(int)\0"
    "qryTimingsClicked()\0freqTableChanged(int)\0"
    "freqChannelChanged(int)\0freqChanged(double)\0"
    "freqRfChanged(double)\0audioModeChanged(int)\0"
    "detectSubchansClicked()\0stereoModeChanged()\0"
    "rdsModeChanged()\0vidCapFormatChanged(int)\0"
    "vidFieldChanged(int)\0frameWidthChanged()\0"
    "frameHeightChanged()\0frameSizeChanged(int)\0"
    "frameIntervalChanged(int)\0"
    "vidOutFormatChanged(int)\0"
    "vbiMethodsChanged(int)\0changeAudioDevice()\0"
    "changePixelAspectRatio()\0cropChanged()\0"
    "composeChanged()\0colorspaceChanged(int)\0"
    "xferFuncChanged(int)\0ycbcrEncChanged(int)\0"
    "quantRangeChanged(int)\0"
};

void GeneralTab::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GeneralTab *_t = static_cast<GeneralTab *>(_o);
        switch (_id) {
        case 0: _t->audioDeviceChanged(); break;
        case 1: _t->pixelAspectRatioChanged(); break;
        case 2: _t->croppingChanged(); break;
        case 3: _t->clearBuffers(); break;
        case 4: _t->inputChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->outputChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->inputAudioChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->outputAudioChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->standardChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->qryStdClicked(); break;
        case 10: _t->timingsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->qryTimingsClicked(); break;
        case 12: _t->freqTableChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->freqChannelChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->freqChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 15: _t->freqRfChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 16: _t->audioModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->detectSubchansClicked(); break;
        case 18: _t->stereoModeChanged(); break;
        case 19: _t->rdsModeChanged(); break;
        case 20: _t->vidCapFormatChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->vidFieldChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->frameWidthChanged(); break;
        case 23: _t->frameHeightChanged(); break;
        case 24: _t->frameSizeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: _t->frameIntervalChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->vidOutFormatChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->vbiMethodsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 28: _t->changeAudioDevice(); break;
        case 29: _t->changePixelAspectRatio(); break;
        case 30: _t->cropChanged(); break;
        case 31: _t->composeChanged(); break;
        case 32: _t->colorspaceChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->xferFuncChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 34: _t->ycbcrEncChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 35: _t->quantRangeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GeneralTab::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GeneralTab::staticMetaObject = {
    { &QGridLayout::staticMetaObject, qt_meta_stringdata_GeneralTab,
      qt_meta_data_GeneralTab, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GeneralTab::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GeneralTab::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GeneralTab::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GeneralTab))
        return static_cast<void*>(const_cast< GeneralTab*>(this));
    return QGridLayout::qt_metacast(_clname);
}

int GeneralTab::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGridLayout::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 36)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 36;
    }
    return _id;
}

// SIGNAL 0
void GeneralTab::audioDeviceChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void GeneralTab::pixelAspectRatioChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void GeneralTab::croppingChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void GeneralTab::clearBuffers()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
