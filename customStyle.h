#ifndef CUSTOMSTYLE_H
#define CUSTOMSTYLE_H
#include <QStyle>

class CCustomStyle : public QStyle
{
    Q_OBJECT

public:
    CCustomStyle(const QString& = QString(), const QString& = QString()) ;
    virtual ~CCustomStyle(){}

    void polish(QPalette &palette);
private:
    QString		m_sBackgroundIcon;
    QString		m_sButtonIcon;
    void		changeTexture(QPalette&);
    void		setTexture(QPalette &palette, QPalette::ColorRole role,
                    const QPixmap &pixmap);
};
#endif // CUSTOMSTYLE_H
