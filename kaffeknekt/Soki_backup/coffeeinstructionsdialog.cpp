#include "coffeeinstructionsdialog.h"
#include "ui_coffeinstructionsdialog.h"

#include "QPixmap"
#include "QTransform"
#include "QGridLayout"

CoffeeInstructionsDialog::CoffeeInstructionsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CoffeeInstructionsDialog)
{
    ui ->setupUi(this);
    //dette setter bildet i beans-labele akk som bestemt.
    QPixmap pix(":/icons/beans1.png");
    QTransform transform;
    transform.rotate(90);
    QPixmap rotated = pix.transformed(transform, Qt::SmoothTransformation);
    //setter bildet i beans-label:
    ui->beans->setPixmap(rotated);
    ui->beans->setScaledContents(true);

    //oppretter ny QLabel for tekst:
    QLabel *textLabel = new QLabel(ui->scrollAreaWidgetContents);
    textLabel->setText(R"(
        <h2>☕️ Brewing the Perfect Espresso☕️</h2>
        <p><b>1. Fresh Grind</b><br>Grind fresh beans to a fine, even texture.</p>
        <p><b>2. Precise Dose</b><br>Weigh out 18–20 g of grounds and gently mound them in the portafilter.</p>
        <p><b>3. Even Distribution</b><br>Tap the side of the portafilter to settle the coffee.</p>
        <p><b>4. Clean Leveling</b><br>Sweep off any stray grounds to create a flat surface.</p>
        <p><b>5. Firm Tamp</b><br>Press down with 30–40 lbs of pressure using a level tamper.</p>
        <p><b>6. Secure Lock-In</b><br>Twist the portafilter firmly into the group head.</p>
        <p><b>7. Perfect Pull</b><br>Start the pump and aim for a 25–30 second extraction into a pre-warmed cup.</p>
        <p><b>8. Flow Check</b><br>Watch for a smooth, caramel-colored stream—no drips or spurts.</p>
        <p><b>9. Clean & Reset</b><br>Stop the pump, eject the puck, wipe the basket, and prepare for your next shot.</p>
        <p>✨ Enjoy your coffee ✨</p>
    )");
    textLabel->setStyleSheet("background-color: rgba(255,255,255,0); color: black;");
    textLabel->setWordWrap(true);

    //fikser på størrelsen slik at den kan utvides dersom det trengs ( og det gjør det gitl)*ø
    textLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    //legger til begge grieene ( altså biold eog teksten) i samme celle:
    auto *layout = new QGridLayout(ui->scrollAreaWidgetContents);
    layout->addWidget(ui->beans, 0, 0);
    layout->addWidget(textLabel, 0, 0); //legger seg oppå/aka over bilde liksom :))

    layout->setAlignment(textLabel, Qt::AlignTop);
    layout->setAlignment(ui->beans, Qt::AlignTop);




}

CoffeeInstructionsDialog::~CoffeeInstructionsDialog(){
    delete ui;
}
