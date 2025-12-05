#include <QtQml/qqmlprivate.h>
#include <QtCore/qdir.h>
#include <QtCore/qurl.h>
#include <QtCore/qhash.h>
#include <QtCore/qstring.h>

namespace QmlCacheGeneratedCode {
namespace _qt_qml_AI_0x2d_Girlfriend_Main_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_AI_0x2d_Girlfriend_Components_SideButton_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_AI_0x2d_Girlfriend_Components_HistoryListModel_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_AI_0x2d_Girlfriend_Components_MessageBubble_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_AI_0x2d_Girlfriend_Components_ChatMessageListModel_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_AI_0x2d_Girlfriend_Pages_StatePage_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_AI_0x2d_Girlfriend_Pages_SettingPage_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}
namespace _qt_qml_AI_0x2d_Girlfriend_Pages_GreetPage_qml { 
    extern const unsigned char qmlData[];
    extern const QQmlPrivate::AOTCompiledFunction aotBuiltFunctions[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), &aotBuiltFunctions[0], nullptr
    };
}

}
namespace {
struct Registry {
    Registry();
    ~Registry();
    QHash<QString, const QQmlPrivate::CachedQmlUnit*> resourcePathToCachedUnit;
    static const QQmlPrivate::CachedQmlUnit *lookupCachedUnit(const QUrl &url);
};

Q_GLOBAL_STATIC(Registry, unitRegistry)


Registry::Registry() {
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/AI-Girlfriend/Main.qml"), &QmlCacheGeneratedCode::_qt_qml_AI_0x2d_Girlfriend_Main_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/AI-Girlfriend/Components/SideButton.qml"), &QmlCacheGeneratedCode::_qt_qml_AI_0x2d_Girlfriend_Components_SideButton_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/AI-Girlfriend/Components/HistoryListModel.qml"), &QmlCacheGeneratedCode::_qt_qml_AI_0x2d_Girlfriend_Components_HistoryListModel_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/AI-Girlfriend/Components/MessageBubble.qml"), &QmlCacheGeneratedCode::_qt_qml_AI_0x2d_Girlfriend_Components_MessageBubble_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/AI-Girlfriend/Components/ChatMessageListModel.qml"), &QmlCacheGeneratedCode::_qt_qml_AI_0x2d_Girlfriend_Components_ChatMessageListModel_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/AI-Girlfriend/Pages/StatePage.qml"), &QmlCacheGeneratedCode::_qt_qml_AI_0x2d_Girlfriend_Pages_StatePage_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/AI-Girlfriend/Pages/SettingPage.qml"), &QmlCacheGeneratedCode::_qt_qml_AI_0x2d_Girlfriend_Pages_SettingPage_qml::unit);
    resourcePathToCachedUnit.insert(QStringLiteral("/qt/qml/AI-Girlfriend/Pages/GreetPage.qml"), &QmlCacheGeneratedCode::_qt_qml_AI_0x2d_Girlfriend_Pages_GreetPage_qml::unit);
    QQmlPrivate::RegisterQmlUnitCacheHook registration;
    registration.structVersion = 0;
    registration.lookupCachedQmlUnit = &lookupCachedUnit;
    QQmlPrivate::qmlregister(QQmlPrivate::QmlUnitCacheHookRegistration, &registration);
}

Registry::~Registry() {
    QQmlPrivate::qmlunregister(QQmlPrivate::QmlUnitCacheHookRegistration, quintptr(&lookupCachedUnit));
}

const QQmlPrivate::CachedQmlUnit *Registry::lookupCachedUnit(const QUrl &url) {
    if (url.scheme() != QLatin1String("qrc"))
        return nullptr;
    QString resourcePath = QDir::cleanPath(url.path());
    if (resourcePath.isEmpty())
        return nullptr;
    if (!resourcePath.startsWith(QLatin1Char('/')))
        resourcePath.prepend(QLatin1Char('/'));
    return unitRegistry()->resourcePathToCachedUnit.value(resourcePath, nullptr);
}
}
int QT_MANGLE_NAMESPACE(qInitResources_qmlcache_appAI_Girlfriend)() {
    ::unitRegistry();
    return 1;
}
Q_CONSTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(qInitResources_qmlcache_appAI_Girlfriend))
int QT_MANGLE_NAMESPACE(qCleanupResources_qmlcache_appAI_Girlfriend)() {
    return 1;
}
