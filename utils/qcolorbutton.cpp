
/* qt header */
#include <QColorDialog>

/* local header */
#include "qcolorbutton.h"

QColorButton::QColorButton( QWidget *parent )
        : QPushButton( parent ),
        mEnableColorText( true ),
        mEnableToolTip( true ) {

    connect( this, SIGNAL( clicked() ), SLOT( slotChangeColor() ) );//如果单击按钮执行slotChangeColor
    setCurrentColor( "#ffffff" );
}

//设置当前颜色
void QColorButton::setCurrentColor( const QColor &currentColor )
{

    //如果没有改变颜色则返回
    if ( mCurrentColor == currentColor )
        return;

    mCurrentColor = currentColor;//更新mCurrentColor变量
    if ( mEnableColorText )
        setColorText( mCurrentColor.name() );//如#336699
    if ( mEnableToolTip )
        setToolTip( mCurrentColor.name() );

    QPixmap pix( 16, 16 );//16*16
    pix.fill( mCurrentColor );//用当前色填充
    setIcon( pix );//设置按钮图标
}

void QColorButton::setText( const QString &text ) {

    if ( mRealText == text )
        return;

    mRealText = text;
    QPushButton::setText( mRealText );
}

//开启颜色值文本
void QColorButton::enableColorText( bool enableColorText ) {

    if ( mEnableColorText == enableColorText )
        return;

    mEnableColorText = enableColorText;
    if ( mEnableColorText )
        setColorText( mCurrentColor.name() );
    else
        setText( mRealText );
}

//开启tooltip
void QColorButton::enableToolTip( bool enableToolTip ) {

    if ( mEnableToolTip == enableToolTip )
        return;

    mEnableToolTip = enableToolTip;
    if ( mEnableToolTip )
        setToolTip( mCurrentColor.name() );//设置tooltip为颜色值
    else
        setToolTip( "" );
}

//设置颜色文本
void QColorButton::setColorText( const QString &colorText ) {

    //如果文本没有改变则返回
    if ( text() == colorText )
        return;

    QPushButton::setText( colorText );//明确调用父类的方法
}

//自定义的slot
void QColorButton::slotChangeColor() {

    //弹出颜色对话框，初始值设置为mCurrentColor
    QColor newColor = QColorDialog::getColor( mCurrentColor, this , "Pick a Color", QColorDialog::ShowAlphaChannel);
    if ( newColor.isValid() )
        setCurrentColor( newColor );//如果颜色值合法
}
