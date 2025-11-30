/****************************************************************************
** Meta object code from reading C++ file 'configmgr.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../configmgr.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'configmgr.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN9ConfigMgrE_t {};
} // unnamed namespace

template <> constexpr inline auto ConfigMgr::qt_create_metaobjectdata<qt_meta_tag_ZN9ConfigMgrE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ConfigMgr",
        "userNameChanged",
        "",
        "passwordChanged",
        "uidChanged",
        "asrServerUrlChanged",
        "llmServerUrlChanged",
        "llmModelNameChanged",
        "ttsServerUrlChanged",
        "userName",
        "password",
        "uid",
        "asrServerUrl",
        "llmServerUrl",
        "llmModelName",
        "ttsServerUrl"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'userNameChanged'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'passwordChanged'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'uidChanged'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'asrServerUrlChanged'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'llmServerUrlChanged'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'llmModelNameChanged'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'ttsServerUrlChanged'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
        // property 'userName'
        QtMocHelpers::PropertyData<QString>(9, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet | QMC::Final, 0),
        // property 'password'
        QtMocHelpers::PropertyData<QString>(10, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet | QMC::Final, 1),
        // property 'uid'
        QtMocHelpers::PropertyData<QString>(11, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet | QMC::Final, 2),
        // property 'asrServerUrl'
        QtMocHelpers::PropertyData<QString>(12, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet | QMC::Final, 3),
        // property 'llmServerUrl'
        QtMocHelpers::PropertyData<QString>(13, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet | QMC::Final, 4),
        // property 'llmModelName'
        QtMocHelpers::PropertyData<QString>(14, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet | QMC::Final, 5),
        // property 'ttsServerUrl'
        QtMocHelpers::PropertyData<QString>(15, QMetaType::QString, QMC::DefaultPropertyFlags | QMC::Writable | QMC::StdCppSet | QMC::Final, 6),
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ConfigMgr, qt_meta_tag_ZN9ConfigMgrE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ConfigMgr::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9ConfigMgrE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9ConfigMgrE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9ConfigMgrE_t>.metaTypes,
    nullptr
} };

void ConfigMgr::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ConfigMgr *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->userNameChanged(); break;
        case 1: _t->passwordChanged(); break;
        case 2: _t->uidChanged(); break;
        case 3: _t->asrServerUrlChanged(); break;
        case 4: _t->llmServerUrlChanged(); break;
        case 5: _t->llmModelNameChanged(); break;
        case 6: _t->ttsServerUrlChanged(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ConfigMgr::*)()>(_a, &ConfigMgr::userNameChanged, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConfigMgr::*)()>(_a, &ConfigMgr::passwordChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConfigMgr::*)()>(_a, &ConfigMgr::uidChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConfigMgr::*)()>(_a, &ConfigMgr::asrServerUrlChanged, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConfigMgr::*)()>(_a, &ConfigMgr::llmServerUrlChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConfigMgr::*)()>(_a, &ConfigMgr::llmModelNameChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (ConfigMgr::*)()>(_a, &ConfigMgr::ttsServerUrlChanged, 6))
            return;
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast<QString*>(_v) = _t->userName(); break;
        case 1: *reinterpret_cast<QString*>(_v) = _t->password(); break;
        case 2: *reinterpret_cast<QString*>(_v) = _t->uid(); break;
        case 3: *reinterpret_cast<QString*>(_v) = _t->asrServerUrl(); break;
        case 4: *reinterpret_cast<QString*>(_v) = _t->llmServerUrl(); break;
        case 5: *reinterpret_cast<QString*>(_v) = _t->llmModelName(); break;
        case 6: *reinterpret_cast<QString*>(_v) = _t->ttsServerUrl(); break;
        default: break;
        }
    }
    if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setUserName(*reinterpret_cast<QString*>(_v)); break;
        case 1: _t->setPassword(*reinterpret_cast<QString*>(_v)); break;
        case 2: _t->setUid(*reinterpret_cast<QString*>(_v)); break;
        case 3: _t->setAsrServerUrl(*reinterpret_cast<QString*>(_v)); break;
        case 4: _t->setLlmServerUrl(*reinterpret_cast<QString*>(_v)); break;
        case 5: _t->setLlmModelName(*reinterpret_cast<QString*>(_v)); break;
        case 6: _t->setTtsServerUrl(*reinterpret_cast<QString*>(_v)); break;
        default: break;
        }
    }
}

const QMetaObject *ConfigMgr::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConfigMgr::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9ConfigMgrE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ConfigMgr::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void ConfigMgr::userNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ConfigMgr::passwordChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ConfigMgr::uidChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void ConfigMgr::asrServerUrlChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ConfigMgr::llmServerUrlChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void ConfigMgr::llmModelNameChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void ConfigMgr::ttsServerUrlChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
