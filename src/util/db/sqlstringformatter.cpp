#include "util/db/sqlstringformatter.h"

#include <QSqlDriver>
#include <QSqlField>

#include "util/assert.h"


QString SqlStringFormatter::format(
        const QSqlDatabase& database,
        const QString& value) {
    QSqlDriver* pDriver = database.driver();
    VERIFY_OR_DEBUG_ASSERT(pDriver != nullptr) {
        return value; // unformatted
    }
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QSqlField stringField{QString(), QMetaType(QMetaType::QString)};
#else
    QSqlField stringField(QString(), QVariant::String);
#endif
    stringField.setValue(value);
    return pDriver->formatValue(stringField);
}

namespace {
    template<typename T> // forward-iterable container type with values of type QString
    QString formatListValues(
            const QSqlDatabase& database,
            const T& values) {
        QString result;
        QString previousString;
        for (const QString& value: values) {
            if (!previousString.isEmpty()) {
                result.append(',');
            }
            previousString = SqlStringFormatter::format(database, value);
            result += previousString;
        }
        return result;
    }
    } // namespace

QString SqlStringFormatter::formatList(
        const QSqlDatabase& database,
        const QStringList& values) {
    return formatListValues(database, values);
}

QString SqlStringFormatter::formatList(
        const QSqlDatabase& database,
        const QSet<QString>& values) {
    return formatListValues(database, values);
}
