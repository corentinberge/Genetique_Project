close all
clear all


fichier_out = load( '-ascii', "out.txt");
fichier_matrice = load('-ascii', "A_Genetique_Profil.txt");

L = fichier_matrice(:,1);
Y = fichier_out(:,1);
B = fichier_out(:,2);
LambdaZero = fichier_out(:,3);
Delta = fichier_out(:,4);
Y = mean(Y)
B = mean(B)
LambdaZero = mean(LambdaZero)
Delta = mean(Delta)
J = Y + B*(i./((L - LambdaZero).*(L - LambdaZero) + (Delta/2)^2));
%J = Y + B*(i/((Lambda - LambdaZero).^2 + (Delta/2)^2))
plot(L, abs(J), L, fichier_matrice(:, 3))
