#include <qdom.h>
#include <qfile.h>
#include <qxml.h>
#include <qstring.h>

#include "../lib/vardb.h"


main()
{
  QDomDocument doc("vardb");
  QFile file("VarDB.xml");

  if ( !file.open( IO_ReadOnly ) )
    return(1);

  if ( !doc.setContent(&file) ) {
    file.close();
    return(1);
  }

  file.close();


  QDomElement	docElem = doc.documentElement();
  QDomNode	n = docElem.firstChild();

  while( !n.isNull() )
  {
    QDomElement e = n.toElement(); // try to convert the node to an element.

    if ( !e.isNull() )
    {
      cout << e.tagName() << "=" << e.attribute("name") << endl;

      if (e.hasChildNodes())
      {
        QDomNode	ch = e.firstChild();

        for (ch = e.firstChild(); !ch.isNull(); ch = ch.nextSibling() )
        {
          QDomElement e1 = ch.toElement();
	  cout << "  " << e1.tagName() << "=" << e1.text() << endl;
        }
      }
    }
    else
      cout << "NULL\n";

    n = n.nextSibling();
  }


/*
  struct var_v2	v;

  printf("%u\n", (unsigned)&v.FloatRange - (unsigned)&v);
*/
}
