# Parcels-Collection-System

This is a *C++ program* developed for users to collect or drop off parcels with minimal human interaction. In this program, *linked list* data structure has been used to store the data and *bubble sort* has been used to sort the data, such as the details of locker reservation.

To start using the system to collect parcels, the user needs to have an account. In the Register page, the user will need to provide a username and password. Username should be unique among all the users and without any spacings. If the user has an account, just login in the Login page with the username and password. 

When the user reserves a locker, a 30-days period is given. If no parcels are delivered to that locker within that period, the reservation will be cancelled automatically. Likewise, when the parcel is delivered to the locker, a 14-days period is given. If the user does not collect the parcel within that period, the reservation will be cancelled automatically and the parcel in that locker will be sent to the warehouse. 

When the parcel is delivered to the locker, a notification will be popped out when the user logs in again. The user will be given a 6-digit unique code to retrieve the parcel. The code can be found in the “Status of locker(s)” section. 

Furthermore, when courier staffs deliver the parcel, they do not need a user account. They will need to select the “Drop off” option, and specify which locker they are delivering to, and the parcel will be successfully delivered.  

**Screenshots of program**
![a](https://user-images.githubusercontent.com/93151327/200510347-f664e67f-3a8b-4bfc-bf18-e684bda22c6d.jpg)
![b](https://user-images.githubusercontent.com/93151327/200510361-fd4f4709-a100-4055-9dc8-331a9526c72f.jpg)
![c](https://user-images.githubusercontent.com/93151327/200510382-c22a9b6a-47dc-4e45-b2a1-e736ed6b80ce.jpg)
![d](https://user-images.githubusercontent.com/93151327/200510395-9e70b6c7-426b-4ef8-b800-bcb74d8a42ed.jpg)
