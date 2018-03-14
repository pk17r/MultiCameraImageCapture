/****************************************************************************
** Meta object code from reading C++ file 'qv4l2.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "qv4l2.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qv4l2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ApplicationWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      47,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   32,   32,   32, 0x08,
      33,   32,   32,   32, 0x08,
      51,   32,   32,   32, 0x08,
      66,   32,   32,   32, 0x08,
      80,   32,   32,   32, 0x08,
      95,   32,   32,   32, 0x08,
     116,   32,   32,   32, 0x08,
     127,   32,   32,   32, 0x08,
     138,   32,   32,   32, 0x08,
     150,   32,   32,   32, 0x08,
     161,   32,   32,   32, 0x08,
     175,   32,   32,   32, 0x08,
     189,   32,   32,   32, 0x08,
     203,   32,   32,   32, 0x08,
     225,   32,   32,   32, 0x08,
     243,   32,   32,   32, 0x08,
     265,   32,   32,   32, 0x08,
     283,   32,   32,   32, 0x08,
     303,   32,   32,   32, 0x08,
     313,   32,   32,   32, 0x08,
     326,   32,   32,   32, 0x08,
     342,  363,   32,   32, 0x08,
     365,   32,   32,   32, 0x08,
     383,   32,   32,   32, 0x08,
     404,  424,   32,   32, 0x08,
     431,   32,   32,   32, 0x08,
     456,   32,   32,   32, 0x08,
     473,   32,   32,   32, 0x08,
     488,   32,   32,   32, 0x08,
     496,  532,   32,   32, 0x08,
     534,  532,   32,   32, 0x08,
     568,  532,   32,   32, 0x08,
     602,  532,   32,   32, 0x08,
     633,  657,   32,   32, 0x08,
     661,  657,   32,   32, 0x08,
     685,  657,   32,   32, 0x08,
     710,  657,   32,   32, 0x08,
     733,  657,   32,   32, 0x08,
     756,  657,   32,   32, 0x08,
     775,  657,   32,   32, 0x08,
     794,  657,   32,   32, 0x08,
     823,  657,   32,   32, 0x08,
     847,  657,   32,   32, 0x08,
     874,  657,   32,   32, 0x08,
     901,  657,   32,   32, 0x08,
     924,   32,  947,   32, 0x08,
     953,   32,   32,   32, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ApplicationWindow[] = {
    "ApplicationWindow\0closeDevice()\0\0"
    "closeCaptureWin()\0capStart(bool)\0"
    "capStep(bool)\0outStart(bool)\0"
    "makeFullScreen(bool)\0capFrame()\0"
    "outFrame()\0ctrlEvent()\0snapshot()\0"
    "capVbiFrame()\0capSdrFrame()\0saveRaw(bool)\0"
    "setRenderMethod(bool)\0setBlending(bool)\0"
    "setLinearFilter(bool)\0traceIoctls(bool)\0"
    "changeAudioDevice()\0opendev()\0"
    "openrawdev()\0ctrlAction(int)\0"
    "openRawFile(QString)\0s\0rejectedRawFile()\0"
    "setAudioBufferSize()\0enableScaling(bool)\0"
    "enable\0updatePixelAspectRatio()\0"
    "updateCropping()\0clearBuffers()\0about()\0"
    "overrideColorspaceChanged(QAction*)\0"
    "a\0overrideXferFuncChanged(QAction*)\0"
    "overrideYCbCrEncChanged(QAction*)\0"
    "overrideQuantChanged(QAction*)\0"
    "testPatternChanged(int)\0val\0"
    "horMovementChanged(int)\0"
    "vertMovementChanged(int)\0"
    "showBorderChanged(int)\0showSquareChanged(int)\0"
    "insSAVChanged(int)\0insEAVChanged(int)\0"
    "videoAspectRatioChanged(int)\0"
    "limRGBRangeChanged(int)\0"
    "fillPercentageChanged(int)\0"
    "alphaComponentChanged(int)\0"
    "applyToRedChanged(int)\0tpgDefaultColorspace()\0"
    "__u32\0tpgColorspaceChanged()\0"
};

void ApplicationWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ApplicationWindow *_t = static_cast<ApplicationWindow *>(_o);
        switch (_id) {
        case 0: _t->closeDevice(); break;
        case 1: _t->closeCaptureWin(); break;
        case 2: _t->capStart((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->capStep((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->outStart((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->makeFullScreen((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->capFrame(); break;
        case 7: _t->outFrame(); break;
        case 8: _t->ctrlEvent(); break;
        case 9: _t->snapshot(); break;
        case 10: _t->capVbiFrame(); break;
        case 11: _t->capSdrFrame(); break;
        case 12: _t->saveRaw((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->setRenderMethod((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->setBlending((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->setLinearFilter((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->traceIoctls((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 17: _t->changeAudioDevice(); break;
        case 18: _t->opendev(); break;
        case 19: _t->openrawdev(); break;
        case 20: _t->ctrlAction((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->openRawFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 22: _t->rejectedRawFile(); break;
        case 23: _t->setAudioBufferSize(); break;
        case 24: _t->enableScaling((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 25: _t->updatePixelAspectRatio(); break;
        case 26: _t->updateCropping(); break;
        case 27: _t->clearBuffers(); break;
        case 28: _t->about(); break;
        case 29: _t->overrideColorspaceChanged((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 30: _t->overrideXferFuncChanged((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 31: _t->overrideYCbCrEncChanged((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 32: _t->overrideQuantChanged((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 33: _t->testPatternChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 34: _t->horMovementChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 35: _t->vertMovementChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: _t->showBorderChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: _t->showSquareChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 38: _t->insSAVChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 39: _t->insEAVChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 40: _t->videoAspectRatioChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 41: _t->limRGBRangeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 42: _t->fillPercentageChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 43: _t->alphaComponentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 44: _t->applyToRedChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 45: { __u32 _r = _t->tpgDefaultColorspace();
            if (_a[0]) *reinterpret_cast< __u32*>(_a[0]) = _r; }  break;
        case 46: _t->tpgColorspaceChanged(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ApplicationWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ApplicationWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ApplicationWindow,
      qt_meta_data_ApplicationWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ApplicationWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ApplicationWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ApplicationWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ApplicationWindow))
        return static_cast<void*>(const_cast< ApplicationWindow*>(this));
    if (!strcmp(_clname, "cv4l_fd"))
        return static_cast< cv4l_fd*>(const_cast< ApplicationWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ApplicationWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 47)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 47;
    }
    return _id;
}
static const uint qt_meta_data_SaveDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   29,   31,   31, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_SaveDialog[] = {
    "SaveDialog\0selected(QString)\0s\0\0"
};

void SaveDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SaveDialog *_t = static_cast<SaveDialog *>(_o);
        switch (_id) {
        case 0: _t->selected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SaveDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SaveDialog::staticMetaObject = {
    { &QFileDialog::staticMetaObject, qt_meta_stringdata_SaveDialog,
      qt_meta_data_SaveDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SaveDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SaveDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SaveDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SaveDialog))
        return static_cast<void*>(const_cast< SaveDialog*>(this));
    return QFileDialog::qt_metacast(_clname);
}

int SaveDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFileDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
