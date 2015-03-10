Attribute VB_Name = "SimCommon"
DefInt I-J
Option Base 1 ' Set default array subscripts to 1.
' Arrays used by TPRPRFDC passed through COMMON
' Public Zraob!(1 To 200), RHraob!(1 To 200), Praob!(1 To 200), Traob!(1 To 200)
Public ESV#(52)

Public PRmin%    'RAOB must go higher than PRmin to be used (eg 25 mb)
'Public Htop%     'Limit for upward integration (normally 50 km)
'Public NTB%      'Total number of frequencies = Nel*Nlo*Nnu
Public MonthMin% 'First month to use (1-12)
Public MonthMax% 'Last month to use (1-12 and >MonthMin)

Public NraobMax  'Maximum number of RAOBs to use

Public i%, j%, k%


Public Pr!(1 To 12) ', Zr!(1 To 12)
Public ElUP!(1 To 5), ElDN(1 To 5)
Public Alt$




