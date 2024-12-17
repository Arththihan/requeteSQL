#include "widget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtDebug>
#include <QComboBox>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // Création d'un layout vertical pour organiser les widgets
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Configuration de la connexion à la base de données MySQL
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("foot");
    db.setUserName("etudiant");
    db.setPassword("Passciel");

    // Vérification de l'ouverture de la base de données
    if (db.open()) {
        qDebug() << "Connexion réussie à la base de données" << db.databaseName();

        QSqlQuery query;

        // Exécuter une requête SQL pour récupérer les données
        if (query.exec("SELECT id, nom, club, note FROM jeu")) {
            // Création d'un tableau
            QTableWidget* tableWidget = new QTableWidget(this);
            layout->addWidget(tableWidget);

            // Configuration du tableau
            tableWidget->setColumnCount(4);
            tableWidget->setHorizontalHeaderLabels({"ID", "Nom", "Club", "Note"});

            int row = 0;


            while (query.next()) {
                int id = query.value(0).toInt(); // Récupérer l'ID
                tableWidget->insertRow(row);

                // Remplir les colonnes de la ligne avec les valeurs de la BDD
                tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(id))); // Colonne ID
                tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString())); // Colonne Nom
                tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString())); // Colonne Club

                // Création d'une QComboBox pour modifier la note
                QComboBox* comboBox = new QComboBox(this);
                comboBox->addItems({"1", "2", "3", "4", "5"}); // valeur de la note jusqu'à "5"
                comboBox->setCurrentText(query.value(3).toString()); // valeur actuelle de la note

                // mettre à jour la valeur de la note
                connect(comboBox, &QComboBox::currentTextChanged, this, [=](const QString &newNote) {
                    // Vérifier que la base de données est toujours ouverte avant l'opération
                    if (!db.isOpen()) {
                        qDebug() << "Erreur: La base de données n'est pas ouverte.";
                        return;
                    }

                    // Préparer et exécuter la requête de mise à jour
                    QSqlQuery updateQuery;
                    updateQuery.prepare("UPDATE jeu SET note = :note WHERE id = :id");
                    updateQuery.bindValue(":note", newNote); // Nouvelle valeur de la note
                    updateQuery.bindValue(":id", id);        // ID correspondant dans la base de données

                    if (updateQuery.exec()) {
                        qDebug() << "Note mise à jour avec succès pour l'ID:" << id << "Nouvelle note:" << newNote;
                    } else {
                        qDebug() << "Erreur lors de la mise à jour de la note pour l'ID:" << id
                                 << " - Erreur: " << updateQuery.lastError().text();
                    }
                });

                // Ajouter la QComboBox dans la cellule de la colonne "Note"
                tableWidget->setCellWidget(row, 3, comboBox);
                ++row;
            }

            qDebug() << "Requête exécutée avec succès !";

        } else {
            // En cas d'erreur lors de l'exécution de la requête SQL
            qDebug() << "Erreur lors de l'exécution de la requête : " << query.lastError().text();
            QLabel* errorLabel = new QLabel("Erreur lors de l'exécution de la requête : " + query.lastError().text());
            layout->addWidget(errorLabel); // Afficher un message d'erreur dans l'interface
        }

    } else {
        // En cas d'échec de la connexion à la base de données
        qDebug() << "Échec de la connexion à la base de données : " << db.lastError().text();
        QLabel* errorLabel = new QLabel("Échec de la connexion à la base de données : " + db.lastError().text());
        layout->addWidget(errorLabel); // Afficher un message d'erreur dans l'interface
    }

    // Définir le layout principal pour le widget
    setLayout(layout);
}


Widget::~Widget()
{

}
