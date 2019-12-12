close all
clear all

Y = 0.22
B = 3
LambdaZero= 6562
Delta = 20
fichier_matrice = load( '-ascii', "A_Genetique_Profil.txt");


Lambda = fichier_matrice(:,1);
L = Lambda';
J = Y + B*(i./((L - LambdaZero).*(L - LambdaZero) + (Delta/2)^2));
%J = Y + B*(i/((Lambda - LambdaZero).^2 + (Delta/2)^2))
plot(Lambda, abs(fichier_matrice(:,2) + fichier_matrice(:,3)*i))
