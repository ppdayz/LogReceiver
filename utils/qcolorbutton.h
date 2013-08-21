#ifndef QCOLORBUTTON_H
#define QCOLORBUTTON_H

/* qt header */
#include <QPushButton>

/**
 * Colorbutton,选择和显示实际的颜色.
 *
 * @brief Choose color via QPushButton and display it.
 * @author puyz
 * @date 2013-08-11
 * @since 1.0.0
 * @version 1.0.0
 */
class QColorButton : public QPushButton
{

    Q_OBJECT
    Q_CLASSINFO( "author", "puyz" )
    Q_CLASSINFO( "email", "puyz@leadontec.com" )
    Q_CLASSINFO( "company", "Leadontec" )
    Q_CLASSINFO( "date", "2013-08-11" )
    Q_PROPERTY(bool isColorTextEnabled READ isColorTextEnabled WRITE enableColorText)

public:
    /**
     * QColorButton的缺省构造函数.
     *
     * @param parent - 父级widget.
     * @since 1.0.0
     */
    QColorButton( QWidget *parent = 0 );

    /**
     * 设置当前颜色.
     *
     * @param color -要设置的颜色.
     * @since 1.0.0
     */
    void setCurrentColor( const QColor &color );

    /**
     * 返回当前颜色.
     *
     * @return 当前颜色.
     * @since 1.0.0
     */
    inline QColor currentColor() const { return mCurrentColor; }

    /**
     * 设置按钮文本.
     *
     * @param text     - 要设置的文本.
     * @since 1.0.0
     */
    void setText( const QString &text );

    /**
     * Enable- 如果开启则显示用户定义的文本否则显示颜色值
     *
     * @param enableColorText -    True,则显示用户定义的文本;否则false显示颜色值.
     * 缺省为 true.
     * @since 1.0.0
     */
    void enableColorText( bool enableColorText );

    /**
     * 是否设置了用户定义文本还是颜色值?
     *
     * @如果设置用户定义文本则返回True;否则如果开启颜色值则返回false.
     *
     * @since 1.0.0
     */
    inline bool isColorTextEnabled() const { return mEnableColorText; }

    /**
     * Enable-显示颜色值的ToolTip.
     *
     * @param enableToolTip    - True, 开启显示颜色值的ToolTip - 没开启为false
     * 缺省为true.
     * @since 1.0.0
     */
    void enableToolTip( bool enableToolTip );

    /**
     * 颜色值显示的tooltip是否开启?
     *
     * @return True,-如果开启 - 否则 false.
     * @since 1.0.0
     */
    inline bool isToolTipEnabled() const { return mEnableToolTip; }

private:
    /**
     * Intern function to set the new or changed color value as text.
     *
     * @param colorText     The color value as text.
     * @since 1.0.0
     */
    void setColorText( const QString &colorText );

    /**
     *颜色文本是否开启?
     *
     * @since 1.0.0
     */
    bool mEnableColorText;

    /**
     * 是否开启了color Tooltip?
     *
     * @since 1.0.0
     */
    bool mEnableToolTip;

    /**
     * 当前颜色.
     *
     * @since 1.0.0
     */
    QColor mCurrentColor;

    /**
     * 实际的文本.
     *
     * @since 1.0.0
     */
    QString mRealText;

private slots:
    /**
     * 内部slot用来调用colordialog并修改颜色
     *
     * @since 1.0.0
     */
    void slotChangeColor();
};

#endif
