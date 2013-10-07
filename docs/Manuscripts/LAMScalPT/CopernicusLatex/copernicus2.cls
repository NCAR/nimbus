%% File: copernicus2.cls
%% ---------------------------------
%% The LaTeX2e class file for journals published by Copernicus Publications (Copernicus GmbH)
%% E-mail:  publications@copernicus.org
%% URL:     http://publications.copernicus.org
%% ---------------------------------
%% Author:
%% Dieter Schmitt
%% Max-Planck-Institut f\"ur Sonnensystemforschung
%% Max-Planck-Str. 2
%% D-37191 Katlenburg-Lindau
%% Germany
%% E-mail: schmitt@mps.mpg.de
%%
%% for Copernicus Publications (Copernicus GmbH).
%%
%% With major contributions from Martin Rasmussen and Nadine Deisel.
%%
%% ---------------------------------
%% This file may be used for non-profit purposes. 
%% It may not be distributed in exchange of money.
%% The author provides it 'as is' and does not
%% guarantee it in any way.
%% ---------------------------------
%% This class file consists of several modules, each with its separate copyright
%% The modules that are specific to Copernicus publications are
%% Copyright (C) Copernicus GmbH
%%
%% The entire package is available from
%% http://publications.copernicus.org/for_authors/latex_instructions.html
%% ---------------------------------
%% Version 5.7, 2013/05/06

\NeedsTeXFormat{LaTeX2e}[1995/06/01]
\ProvidesClass{copernicus2}[2013/05/06 5.7 (DS)]
\def\clsversion{5.7}

\newif\ifasr
\newif\ifdwes
\newif\ifessd
\newif\ifesurf
\newif\ifgh
\newif\ifgtes
\newif\ifhgss
\newif\ifjsss
\newif\ifms
\newif\ifprp
%\newif\ifsg
\newif\ifwe

\newif\ifcopyediting

\newif\if@nohyperref
\newif\if@noauthor
\newif\if@noref
\newif\if@nolastpage
\newif\if@cop@home
\newif\if@bar
\newif\if@twostagejnl

\DeclareOption{asr}{\asrtrue\@bartrue\@twostagejnlfalse}
\DeclareOption{dwes}{\dwestrue\@barfalse\@twostagejnltrue}
\DeclareOption{essd}{\essdtrue\@barfalse\@twostagejnltrue}
\DeclareOption{esurf}{\esurftrue\@barfalse\@twostagejnltrue}
\DeclareOption{gh}{\ghtrue\@bartrue\@twostagejnlfalse}
\DeclareOption{gtes}{\gtestrue\@bartrue\@twostagejnlfalse}
\DeclareOption{hgss}{\hgsstrue\@bartrue\@twostagejnlfalse}
\DeclareOption{jsss}{\jssstrue\@bartrue\@twostagejnlfalse}
\DeclareOption{ms}{\mstrue\@barfalse\@twostagejnlfalse}
\DeclareOption{prp}{\prptrue\@bartrue\@twostagejnlfalse}
\DeclareOption{sg}{\sgtrue\@barfalse\@twostagejnltrue}
\DeclareOption{we}{\wetrue\@bartrue\@twostagejnlfalse}

\DeclareOption{copyediting}{\copyeditingtrue\@bartrue\@twostagejnlfalse}


\DeclareOption{nohyperref}{\@nohyperreftrue}
\DeclareOption{noauthor}{\@noauthortrue}
\DeclareOption{noref}{\@noreftrue}
\DeclareOption{nolastpage}{\@nolastpagetrue}

\DeclareOption*{\PassOptionsToClass{\CurrentOption}{article}}
\ProcessOptions

\LoadClass[fleqn,twoside,twocolumn]{article}

%>le-tex, 2012-03-16
\ifx\xmltexversion\@undefined\RequirePackage[utf8]{inputenc}\fi
%<le-tex, 2012-03-16

\IfFileExists{copernicuslogo.pdf}{\@cop@hometrue}{\@cop@homefalse}

\if@cop@home
   \if@bar
      \paperheight=283mm
      \paperwidth=213mm
   \else
      \paperheight=277mm
      \paperwidth=210mm
   \fi  
\else
   \paperheight=277mm
   \paperwidth=210mm
\fi

\ifx\pdfoutput\undefined
   \IfFileExists{graphicx.sty}{\RequirePackage[dvips]{graphicx}% LaTeX
      \DeclareGraphicsExtensions{.eps,.ps}}{%
      \ClassWarningNoLine{copernicus2}{Cannot find graphicx.sty; proceeding without it}}
   \RequirePackage[dvips]{color}
\else
   \ifnum\pdfoutput=0
      \IfFileExists{graphicx.sty}{\RequirePackage[dvips]{graphicx}% LaTeX
         \DeclareGraphicsExtensions{.eps,.ps}}{%
         \ClassWarningNoLine{copernicus2}{Cannot find graphicx.sty; proceeding without it}}
      \RequirePackage[dvips]{color}
   \else
      \IfFileExists{graphicx.sty}{\RequirePackage[pdftex]{graphicx}% pdfLaTeX
         \DeclareGraphicsExtensions{.pdf,.png,.jpg}}{%
         \ClassWarningNoLine{copernicus2}{Cannot find graphicx.sty; proceeding without it}}
      \RequirePackage[pdftex]{color}
      \pdfinfo{/Creator (copernicus2.cls version \clsversion)}
      \if@cop@home
         \if@bar
            \pdfpagewidth=213mm
            \pdfpageheight=283mm
         \else
            \pdfpagewidth=210mm
            \pdfpageheight=277mm
         \fi
      \else
         \pdfpagewidth=210mm
         \pdfpageheight=277mm
      \fi
   \fi
\fi

\RequirePackage{authblk}
%>le-tex, 2012-04-18
\renewcommand\@author{\ifx\AB@affillist\AB@empty\AB@author\else
      \ifnum\value{affil}>\value{Maxaffil}%\def\rlap##1{##1}%
    \AB@authlist\\[\affilsep]\AB@affillist
    \else  \AB@authors\fi\fi}
%<le-tex, 2012-04-18

%>le-tex, 2012-03-19 (copied here from copernicus.cls [2012/03/16 4.3l (DS)])
\def\ModuleVersion#1[#2]{}
    \ModuleVersion{appendix}
           [1999/02/23 2.1 (PWD)]
\@ifundefined{iffigcaps}{}
  {\PackageWarningNoLine{appendix}
     {***********************************\MessageBreak
      This package read in after `figcaps'\MessageBreak
      There could be problems with sublabelled\MessageBreak
      figure and table numbers.\MessageBreak
      ***********************************}}
\providecommand{\appendixname}{Appendix}
\let\orisection=\section
\let\theolds=\thesection
\let\theoldss=\thesubsection
\def\appendix{\par
  \setcounter{section}{0}
  \setcounter{subsection}{0}
  \def\thesection{\Alph{section}}
  \def\thesubsection{\thesection\arabic{subsection}}
  \setcounter{secnumdepth}{4}
  \@addtoreset{equation}{section}
  \setcounter{equation}{0}
  \def\theequation{\thesection\arabic{equation}}
  \def\@Alph##1{\ifcase##1A\or A\or B\or C\or D\else\@Ialph{##1}\fi}
  \let\old@sect\@sect
  \def\@sect##1##2##3##4##5##6[##7]##8{%
    \ifnum ##2>\c@secnumdepth
      \let\@svsec\@empty
    \else
      \refstepcounter{##1}%
      \protected@edef\@svsec{\@seccntformat{##1}\relax}%
    \fi
    \@tempskipa ##5\relax
    \ifdim \@tempskipa>\z@
      \begingroup
        ##6{%
          \ifnum ##2=\@ne
             \noindent\appendixname\space\@svsec\par\nobreak
             \vskip\baselineskip\noindent
          \else
            \@hangfrom{\hskip ##3\relax\@svsec}%
          \fi  
          \interlinepenalty \@M ##8\@@par}%
      \endgroup
      \csname ##1mark\endcsname{##7}%
      \addcontentsline{toc}{##1}{%
        \ifnum ##2>\c@secnumdepth \else
          \protect\numberline{\csname the##1\endcsname}%
        \fi
        ##7}%
    \else
      \def\@svsechd{%
        ##6{\hskip ##3\relax
        \@svsec ##8}%
        \csname ##1mark\endcsname{##7}%
        \addcontentsline{toc}{##1}{%
          \ifnum ##2>\c@secnumdepth \else
            \protect\numberline{\csname the##1\endcsname}%
          \fi
          ##7}}%
    \fi
    \@xsect{##5}}}
\def\noappendix{\setcounter{secnumdepth}{0}
  \global\let\section=\orisection
  \global\let\thesection=\theolds
  \global\let\thesubsection=\theoldss
  \let\@sect\old@sect}
%<le-tex, 2012-03-19

\RequirePackage[authoryear,round]{natbib}
\def\NAT@sort{0}\def\NAT@cmprs{0}
\renewcommand\NAT@sep{;} \renewcommand\NAT@cmt{, }
\renewcommand\NAT@aysep{,} \renewcommand\NAT@yrsep{,}
\setlength{\bibsep}{\z@}
\let\bibfont\small
\NAT@numbersfalse
\NAT@set@cites
\let\NAT@set@cites\relax
\newcommand{\urlprefix}{}

\IfFileExists{amssymb.sty}{\RequirePackage{amssymb}}{%
   \ClassWarningNoLine{copernicus2}{Cannot find amssymb.sty; proceeding without it}}
\IfFileExists{amsmath.sty}{\RequirePackage[intlimits,fleqn]{amsmath}}{%
   \ClassWarningNoLine{copernicus2}{Cannot find amsmath.sty; proceeding without it}}%le-tex, 2012-03-16
\IfFileExists{accents.sty}{\RequirePackage{accents}}{%
   \ClassWarningNoLine{copernicus2}{Cannot find accents.sty; proceeding without it}}%le-tex, 2012-03-16
\IfFileExists{array.sty}{\RequirePackage{array}}{%
   \ClassWarningNoLine{copernicus}{Cannot find array.sty; proceeding without it}}%le-tex, 2012-03-19
\IfFileExists{multirow.sty}{\RequirePackage{multirow}}{%
   \ClassWarningNoLine{copernicus2}{Cannot find multirow.sty; proceeding without it}}%le-tex, 2012-03-19
\IfFileExists{txfonts.sty}{\RequirePackage{txfonts}}{%
   \ClassWarningNoLine{copernicus2}{Cannot find txfonts.sty; proceeding without it}}

\if@cop@home\if@nohyperref\else
   \usepackage[pdftex,bookmarks=false,colorlinks]{hyperref}
   \hypersetup{anchorcolor=black,citecolor=black,filecolor=black,linkcolor=black,%
      menucolor=black,pagecolor=black,urlcolor=black}
\fi\fi

\IfFileExists{url.sty}{\RequirePackage{url}\urlstyle{same}}%
   {\ClassWarningNoLine{copernicus2}{Cannot find url.sty; proceeding without it}%
   \newcommand{\url}[1]{\ClassError{copernicus2}%
   {To use \string\url, you must have url.sty}{Install url.sty}}}
   
\IfFileExists{textcomp.sty}%
   {\RequirePackage{textcomp}\def\permil{\ifmmode\text{\textperthousand}\else\textperthousand\fi}}%
   {\ClassWarningNoLine{copernicus2}{Cannot find textcomp.sty; proceeding without it}%
   \newcommand{\permil}{\ensuremath{\kern-.05em%
   \raise.5ex\hbox{\the\scriptfont0 0}%
   \kern-.15em/\kern-.15em\lower.25ex\hbox{\the\scriptfont0 00}}}%
   \def\textmu{\ensuremath{\mu}}}
   
\newcommand{\@journalnameabbreviation}{J. Name}
\newcommand{\@journalnameshort}{JN}
\newcommand{\@journalnameshortlower}{jn}
\newcommand{\@journalurl}{www.jn.net}
\newcommand{\@journalcopyright}{Copyright}
\newcommand{\@journallogo}{Journallogo}
\newcommand{\@sentence}{Sentence.}
\definecolor{textcol}{cmyk}{0.0,0.00,0.00,1.00}
\definecolor{bgcol}{cmyk}{0.00,0.00,0.00,0.00}
\definecolor{barcol}{cmyk}{0.00,0.00,0.00,0.00}
%%
\ifasr
   \renewcommand{\@journalnameabbreviation}{Adv. Sci. Res.}
   \renewcommand{\@journalnameshort}{ASR}
   \renewcommand{\@journalnameshortlower}{asr}
   \renewcommand{\@journalurl}{www.adv-sci-res.net}
   \renewcommand{\@journalcopyright}{\copyright\ Author(s)\ \@pyear. CC Attribution 3.0 License.\\[1mm]
      \includegraphics[width=1.5cm]{CreativeCommons_Attribution_License.png}}
   \renewcommand{\@journallogo}{%
      \parbox[t][\z@][t]{36.0mm}{\vskip-3mm\includegraphics[width=38mm]{ASR_Logo.pdf}\\[-2.1mm] \null\hfill}}
   \renewcommand{\@sentence}{Published by Copernicus Publications.}
   \definecolor{textcol}{cmyk}{1.00,0.75,0.05,0.40}
   \definecolor{bgcol}{cmyk}{0.00,0.00,0.00,0.00}
   \definecolor{barcol}{cmyk}{0.10,0.15,0.50,0.00}
\fi
\ifdwes
   \renewcommand{\@journalnameabbreviation}{Drink. Water Eng. Sci.}
   \renewcommand{\@journalnameshort}{DWES}
   \renewcommand{\@journalnameshortlower}{dwes}
   \renewcommand{\@journalurl}{www.drink-water-eng-sci.net}
   \renewcommand{\@journalcopyright}{\copyright\ Author(s)\ \@pyear. CC Attribution 3.0 License.\\[1mm]
      \includegraphics[width=1.5cm]{CreativeCommons_Attribution_License.png}}
   \renewcommand{\@journallogo}{%
      \parbox[t][\z@][t]{38.5mm}{\vskip-3mm\includegraphics[width=39mm]{DWES_Logo.pdf}\\[-2.1mm] \null\hfill}}
   \renewcommand{\@sentence}{Published by Copernicus Publications on behalf of the Delft University of Technology.}
   \definecolor{textcol}{cmyk}{0.00,0.00,0.00,1.00}
   \definecolor{bgcol}{cmyk}{0.00,0.00,0.00,0.00}
   \definecolor{barcol}{cmyk}{0.00,0.00,0.00,0.00}
\fi
\ifessd
   \renewcommand{\@journalnameabbreviation}{Earth Syst. Sci. Data}
   \renewcommand{\@journalnameshort}{ESSD}
   \renewcommand{\@journalnameshortlower}{essd}
   \renewcommand{\@journalurl}{www.earth-syst-sci-data.net}
   \renewcommand{\@journalcopyright}{\copyright\ Author(s)\ \@pyear. CC Attribution 3.0 License.\\[1mm]
      \includegraphics[width=1.5cm]{CreativeCommons_Attribution_License.png}}
   \renewcommand{\@journallogo}{%
      \parbox[t][\z@][t]{24.0mm}{\vskip-3mm\includegraphics[width=24mm]{ESSD_Logo.pdf}\\[-2.1mm] \null\hfill}}
   \renewcommand{\@sentence}{Published by Copernicus Publications.}
   \definecolor{textcol}{cmyk}{1.00,0.25,0.20,0.40}
   \definecolor{bgcol}{cmyk}{0.00,0.00,0.00,0.00}
   \definecolor{barcol}{cmyk}{0.00,0.00,0.00,0.00}
\fi
\ifesurf
   \renewcommand{\@journalnameabbreviation}{Earth Surf. Dynam.}
   \renewcommand{\@journalnameshort}{ESURF}
   \renewcommand{\@journalnameshortlower}{esurf}
   \renewcommand{\@journalurl}{www.earth-surf-dynam.net}
   \renewcommand{\@journalcopyright}{\copyright\ Author(s)\ \@pyear. CC Attribution 3.0 License.\\[1mm]
      \includegraphics[width=1.5cm]{CreativeCommons_Attribution_License.png}}
   \renewcommand{\@journallogo}{%
      \parbox[t][\z@][t]{51.0mm}{\vskip-3mm\includegraphics[height=17.5mm]{ESURF_Logo.pdf}\\[-2.1mm] \null\hfill}}
   \renewcommand{\@sentence}{Published by Copernicus Publications on behalf of the European Geosciences Union.}
   \definecolor{textcol}{rgb}{0.54,0.55,0.55}
   \definecolor{bgcol}{rgb}{1.,1.,1.}
   \definecolor{barcol}{rgb}{1.,1.,1.}
\fi
\ifgh
   \renewcommand{\@journalnameabbreviation}{Geogr. Helv.}
   \renewcommand{\@journalnameshort}{GH}
   \renewcommand{\@journalnameshortlower}{gh}
   \renewcommand{\@journalurl}{www.geogr-helv.net}
   \renewcommand{\@journalcopyright}{\copyright\ Author(s)\ \@pyear. CC Attribution 3.0 License.\\[1mm]
      \includegraphics[width=1.5cm]{CreativeCommons_Attribution_License.png}}
   \renewcommand{\@journallogo}{%
      \parbox[t][\z@][t]{60.5mm}{\vskip-2.5mm\includegraphics[width=62mm]{GH_Logo_grey50.pdf}\\[-2.1mm] \null\hfill}}
   \renewcommand{\@sentence}{\reset@font\sffamily\small Published by Copernicus Publications for the Geographisch-Ethnographische Gesellschaft Z\"urich \& Association Suisse de G\'eographie.}
   \definecolor{textcol}{cmyk}{0.00,0.05,0.00,0.50}
   \definecolor{bgcol}{cmyk}{0.00,0.00,0.00,0.00}
   \definecolor{barcol}{cmyk}{0.00,0.00,0.00,0.00}
%   \definecolor{textcol}{rgb}{0.93,0.00,0.13}
%   \definecolor{bgcol}{rgb}{1.,1.,1.}
%   \definecolor{barcol}{rgb}{1.,1.,1.}
\fi
\ifgtes
   \renewcommand{\@journalnameabbreviation}{Geoth. Energ. Sci.}
   \renewcommand{\@journalnameshort}{GtES}
   \renewcommand{\@journalnameshortlower}{gtes}
   \renewcommand{\@journalurl}{www.geoth-energ-sci.net}
   \renewcommand{\@journalcopyright}{\copyright\ Author(s)\ \@pyear. CC Attribution 3.0 License.\\[1mm]
      \includegraphics[width=1.5cm]{CreativeCommons_Attribution_License.png}}
   \renewcommand{\@journallogo}{%
      \parbox[t][\z@][t]{56.0mm}{\vskip-4mm\includegraphics[height=16mm]{GtES_Logo.pdf}\\[-2.1mm] \null\hfill}}
   \renewcommand{\@sentence}{Published by Copernicus Publications on behalf of the GtV Service GmbH (GtV).}
   \definecolor{textcol}{rgb}{0.98,0.46,0.02}
   \definecolor{bgcol}{rgb}{1.,1.,1.}
   \definecolor{barcol}{rgb}{1.,1.,1.}
\fi
\ifhgss
   \renewcommand{\@journalnameabbreviation}{Hist. Geo Space Sci.}
   \renewcommand{\@journalnameshort}{HGSS}
   \renewcommand{\@journalnameshortlower}{hgss}
   \renewcommand{\@journalurl}{www.hist-geo-space-sci.net}
   \renewcommand{\@journalcopyright}{\copyright\ Author(s)\ \@pyear. CC Attribution 3.0 License.\\[1mm]
      \includegraphics[width=1.5cm]{CreativeCommons_Attribution_License.png}}
   \renewcommand{\@journallogo}{%
      \parbox[t][\z@][t]{31.0mm}{\vskip-4mm\includegraphics[width=33mm]{HGSS_Logo.pdf}\\[-2.1mm] \null\hfill}}
   \renewcommand{\@sentence}{Published by Copernicus Publications.}
   \definecolor{textcol}{cmyk}{1.00,0.50,0.05,0.00}
   \definecolor{bgcol}{cmyk}{0.00,0.00,0.00,0.00}
   \definecolor{barcol}{cmyk}{0.00,0.00,0.00,0.00}
\fi
\ifjsss
   \renewcommand{\@journalnameabbreviation}{J. Sens. Sens. Syst.}
   \renewcommand{\@journalnameshort}{JSSS}
   \renewcommand{\@journalnameshortlower}{jsss}
   \renewcommand{\@journalurl}{www.j-sens-sens-syst.net}
   \renewcommand{\@journalcopyright}{\copyright\ Author(s)\ \@pyear. CC Attribution 3.0 License.\\[1mm]
      \includegraphics[width=1.5cm]{CreativeCommons_Attribution_License.png}}
   \renewcommand{\@journallogo}{%
      \parbox[t][\z@][t]{67.5mm}{\vskip-4.5mm\includegraphics[width=70mm]{JSSS_Logo.pdf}\\[-2.1mm] \null\hfill}}
   \renewcommand{\@sentence}{Published by Copernicus Publications on behalf of the AMA Association for Sensor Technology (AMA).}
   \definecolor{textcol}{rgb}{0.05,0.30,0.56}
   \definecolor{bgcol}{rgb}{1.,1.,1.}
   \definecolor{barcol}{rgb}{1.,1.,1.}
\fi
\ifms
   \renewcommand{\@journalnameabbreviation}{Mech. Sci.}
   \renewcommand{\@journalnameshort}{MS}
   \renewcommand{\@journalnameshortlower}{ms}
   \renewcommand{\@journalurl}{www.mech-sci.net}
   \renewcommand{\@journalcopyright}{\copyright\ Author(s)\ \@pyear. CC Attribution 3.0 License.\\[1mm]
      \includegraphics[width=1.5cm]{CreativeCommons_Attribution_License.png}}
   \renewcommand{\@journallogo}{%
      \parbox[t][\z@][t]{43.0mm}{\vskip-3mm\includegraphics[width=43mm]{MS_Logo.pdf}\\[-2.1mm] \null\hfill}}
   \renewcommand{\@sentence}{Published by Copernicus Publications.}
   \definecolor{textcol}{rgb}{0.47,0.65,0.80}
   \definecolor{bgcol}{rgb}{1.,1.,1.}
   \definecolor{barcol}{rgb}{0.,0.,0.}
\fi
\ifprp
   \renewcommand{\@journalnameabbreviation}{Pattern Recogn. Phys.}
   \renewcommand{\@journalnameshort}{PRP}
   \renewcommand{\@journalnameshortlower}{prp}
   \renewcommand{\@journalurl}{www.pattern-recogn-phys.net}
   \renewcommand{\@journalcopyright}{\copyright\ Author(s)\ \@pyear. CC Attribution 3.0 License.\\[1mm]
      \includegraphics[width=1.5cm]{CreativeCommons_Attribution_License.png}}
   \renewcommand{\@journallogo}{%
      \parbox[t][\z@][t]{55.0mm}{\vskip-3mm\includegraphics[height=16mm]{PRP_Logo.pdf}\\[-2.1mm] \null\hfill}}
   \renewcommand{\@sentence}{Published by Copernicus Publications.}
   \definecolor{textcol}{rgb}{0.90,0.51,0.18}
   \definecolor{bgcol}{rgb}{1.,1.,1.}
   \definecolor{barcol}{rgb}{1.,1.,1.}
\fi
%\ifsg
%   \renewcommand{\@journalnameabbreviation}{Soc. Geogr.}
%   \renewcommand{\@journalnameshort}{SG}
%   \renewcommand{\@journalnameshortlower}{sg}
%   \renewcommand{\@journalurl}{www.soc-geogr.net}
%   \renewcommand{\@journalcopyright}{\copyright\ Author(s)\ \@pyear. CC Attribution 3.0 License.\\[1mm]
%      \includegraphics[width=1.5cm]{CreativeCommons_Attribution_License.png}}
%   \renewcommand{\@journallogo}{%
%      \parbox[t][\z@][t]{33.0mm}{\vskip-2.5mm\includegraphics[width=33mm]{SG_Logo.pdf}\\[-2.1mm] \null\hfill}}
%   \renewcommand{\@sentence}{Published by Copernicus Publications.}
%   \definecolor{textcol}{cmyk}{0.00,1.00,0.70,0.40}
%   \definecolor{bgcol}{cmyk}{0.00,0.00,0.00,0.00}
%   \definecolor{barcol}{cmyk}{0.00,0.00,0.00,0.00}
%\fi
\ifwe
   \renewcommand{\@journalnameabbreviation}{Web Ecol.}
   \renewcommand{\@journalnameshort}{WE}
   \renewcommand{\@journalnameshortlower}{we}
   \renewcommand{\@journalurl}{www.web-ecol.net}
   \renewcommand{\@journalcopyright}{\copyright\ Author(s)\ \@pyear. CC Attribution 3.0 License.\\[1mm]
      \includegraphics[width=1.5cm]{CreativeCommons_Attribution_License.png}}
   \renewcommand{\@journallogo}{%
      \parbox[t][\z@][t]{26.0mm}{\vskip-8.5mm\includegraphics[width=28mm]{WE_Logo.pdf}\\[-2.1mm] \null\hfill}}
   \renewcommand{\@sentence}{Published by Copernicus Publications on behalf of the European Ecological Federation (EEF).}
   \definecolor{textcol}{rgb}{0.20,0.60,0.20}
   \definecolor{bgcol}{rgb}{1.,1.,1.}
   \definecolor{barcol}{rgb}{1.,1.,1.}
\fi
\ifcopyediting
   \renewcommand{\@sentence}{\reset@font\sffamily\normalsize \textcolor[RGB]{255,149,64}{Copy-Editing Track Changes Document -- No Source for Author's Proof-Reading}}
   \definecolor{textcol}{rgb}{0.0,0.0,0.0}
   \definecolor{bgcol}{rgb}{1.,1.,1.}
   \definecolor{barcol}{gray}{0.8}
\fi
\fboxrule=0.8pt
%\fboxsep=1mm  

\newcommand{\blackbox}{\rule{8mm}{3mm}}
\newcommand{\degree}{\ensuremath{^{\circ}}}
\newcommand{\tophline}{\hline\noalign{\vspace{1mm}}}
\newcommand{\middlehline}{\noalign{\vspace{1mm}}\hline\noalign{\vspace{1mm}}}
\newcommand{\bottomhline}{\noalign{\vspace{1mm}}\hline}
\def\@arraycr{\relax\iffalse{\fi\ifnum 0=`}\fi
 \global\let\@clinecorr\@undefined%le-tex, 2012-03-26
 \@ifstar \@xarraycr \@xarraycr}
\def\@cline#1-#2\@nil{%
  \noalign{%
    \ifx\@clinecorr\@undefined
      \vskip0.5mm%
      \global\let\@clinecorr\relax
    \else
      \vskip-1.5mm%
    \fi}%le-tex, 2012-03-26
  \omit
  \@multicnt#1%
  \advance\@multispan\m@ne
  \ifnum\@multicnt=\@ne\@firstofone{&\omit}\fi
  \@multicnt#2%
  \advance\@multicnt-#1%
  \advance\@multispan\@ne
  \leaders\hrule\@height\arrayrulewidth\hfill
  \cr
  \noalign{%
    \vskip-\arrayrulewidth
    \vskip1.5mm}}%le-tex, 2012-03-16
\newcommand{\hhline}{\noalign{\vspace{1mm}}\hline\noalign{\vspace{1mm}}} 
   
\DeclareRobustCommand*{\vec}[1]{\ensuremath{%
   \mathchoice{\mbox{\boldmath$\displaystyle#1$}}
              {\mbox{\boldmath$\textstyle#1$}}
              {\mbox{\boldmath$\scriptstyle#1$}}
              {\mbox{\boldmath$\scriptscriptstyle#1$}}}}

\def\testbx{bx}
\DeclareRobustCommand*{\chem}[1]{\ensuremath{%
   \mathcode`-="0200\mathcode`\=="003D% no space around "-" and "="
   \ifx\testbx\f@series\mathbf{#1}\else\mathrm{#1}\fi}}
\DeclareRobustCommand*{\unit}[1]{\ensuremath{\def\mu{\mbox{\textmu}}\def~{\,}%
   \ifx\testbx\f@series\mathbf{#1}\else\mathrm{#1}\fi}}

\def\acknowname{Acknowledgement}
\def\acknowsname{Acknowledgements}
\def\refname{References}
\def\abstractname{Abstract}
\def\appendixname{Appendix}
\def\figurename{Figure}
\def\tablename{Table}
\def\fnum@figure{\sffamily\color{textcol}\figurename~\thefigure}
\def\fnum@table{\sffamily\color{textcol}\tablename~\thetable}

\def\@normalsize{\@setsize\normalsize{12pt}\xpt\@xpt
   \abovedisplayskip 10\p@ plus2\p@ minus5\p@
   \belowdisplayskip \abovedisplayskip
   \abovedisplayshortskip  \z@ plus3\p@
   \belowdisplayshortskip  6\p@ plus3\p@ minus3\p@
   \let\@listi\@listI}
\def\small{%
   \@setsize\small{11pt}\ixpt\@ixpt
   \abovedisplayskip 8.5\p@ plus3\p@ minus4\p@
   \belowdisplayskip \abovedisplayskip
   \abovedisplayshortskip \z@ plus2\p@
   \belowdisplayshortskip 4\p@ plus2\p@ minus2\p@
   \def\@listi{\leftmargin\leftmargini
      \topsep 4\p@ plus2\p@ minus2\p@\parsep 2\p@ plus\p@ minus\p@
      \itemsep \parsep}}
\let\footnotesize=\small
\def\scriptsize{\@setsize\scriptsize{8pt}\viipt\@viipt}
\def\tiny{\@setsize\tiny{6pt}\vpt\@vpt}
\def\large{\@setsize\large{14pt}\xiipt\@xiipt}
\def\Large{\@setsize\Large{18pt}\xivpt\@xivpt}
\def\LARGE{\@setsize\LARGE{22pt}\xviipt\@xviipt}
\def\huge{\@setsize\huge{25pt}\pt\@xxpt}
\def\Huge{\@setsize\Huge{30pt}\xxvpt\@xxvpt}
 
\normalsize

\if@cop@home
   \if@bar
      \topmargin=-27mm
      \evensidemargin=-6mm
   \else
      \topmargin=-30mm
      \evensidemargin=-9mm
   \fi
\else
   \topmargin=-30mm
   \evensidemargin=-9mm
\fi
\headheight=20mm
\headsep=5mm
\topskip=12pt
\footskip=30pt
\oddsidemargin=-9mm
\textwidth=177mm
\textheight=54\baselineskip
\advance\textheight by \topskip
\columnsep 7mm
\columnseprule 0pt
\parindent=1em
\parskip 0pt% plus 1pt

\setcounter{totalnumber}{2}
\setcounter{topnumber}{2}
\setcounter{dbltopnumber}{2}
\def\topfraction{1.}
\def\dbltopfraction{1.}
\setcounter{bottomnumber}{0}
\def\textfraction{0}
\def\floatpagefraction{.7}
\def\dblfloatpagefraction{.7}
%>le-tex, 2012-03-16
\IfFileExists{subfloat.sty}{\RequirePackage{subfloat}}{%
   \ClassWarningNoLine{copernicus2}{Cannot find subfloat.sty; proceeding without it}}
\protected@xdef\themainfigure{\thefigure}%
\protected@xdef\themaintable{\thetable}%
%<le-tex, 2012-03-16

%\def\section{\@startsection{section}{1}{\z@}{-3.5ex plus -1ex minus
%   -0.2ex}{2.3ex plus 0.2ex}{\reset@font\sffamily\normalsize\bfseries\color{textcol}}}
%\def\subsection{\@startsection{subsection}{2}{\z@}{-3.25ex plus -1ex
%   minus -0.2ex}{1.5ex plus 0.2ex}{\reset@font\sffamily\normalsize\mdseries\color{textcol}}}
%\def\subsubsection{\@startsection{subsubsection}{3}{\z@}{-3.25ex plus
%   -1ex minus -0.2ex}{1.5ex plus 0.2ex}{\reset@font\sffamily\normalsize\mdseries\color{textcol}}}
\def\section{\@startsection{section}{1}{\z@}{-3.5ex plus -1ex minus
   -0.2ex}{2.3ex plus 0.2ex}{\reset@font\sffamily\normalsize\bfseries
   \mathversion{bold}%le-tex, 2012-03-16
   \noindent\leavevmode\color{textcol}}}
\def\subsection{\@startsection{subsection}{2}{\z@}{-3.25ex plus -1ex
   minus -0.2ex}{1.5ex plus 0.2ex}{\reset@font\sffamily\normalsize\mdseries\noindent\leavevmode\color{textcol}}}
\def\subsubsection{\@startsection{subsubsection}{3}{\z@}{-3.25ex plus
   -1ex minus -0.2ex}{1.5ex plus 0.2ex}{\reset@font\sffamily\normalsize\mdseries\noindent\leavevmode\color{textcol}}}
\let\paragraph=\subsubsection
\let\subparagraph=\paragraph

\@ifundefined{reset@font}{\let\reset@font=\relax}{}
\setcounter{secnumdepth}{3}

\def\desfont{\sffamily\bfseries
  \mathversion{bold}%le-tex, 2012-03-16
  \small}
\def\descriptionlabel#1{\hspace\labelsep \desfont #1.}
\def\description{\list{}{\labelwidth\z@ \leftmargin\z@ \itemindent-\leftmargin
   \let\makelabel\descriptionlabel\reset@font\small}}
\let\enddescription\endlist

\newcommand{\introduction}[1][Introduction]{\section{#1}}
\newcommand{\conclusions}[1][Conclusions]{\section{#1}}

\newlength{\nhsize}

%\let\ofigure=\figure 
%\let\endofigure=\endfigure
%\def\figure{\ofigure[tp]\center\vskip2mm\setlength{\nhsize}{\hsize}\addtolength{\nhsize}{-2\fboxsep}\addtolength{\nhsize}{-2\fboxrule}}
%\def\endfigure{\endcenter\vskip-10\p@\endofigure}
%\@namedef{figure*}{\@dblfloat{figure}[tp]\center\vskip2mm\setlength{\nhsize}{\hsize}\addtolength{\nhsize}{-2\fboxsep}\addtolength{\nhsize}{-2\fboxrule}}
%\@namedef{endfigure*}{\endcenter\vskip-10\p@\end@dblfloat}
%
%\let\otable=\table
%\let\endotable=\endtable
%\def\table{\otable[tp]\center\vskip-1mm\setlength{\nhsize}{\hsize}\addtolength{\nhsize}{-2\fboxsep}\addtolength{\nhsize}{-2\fboxrule}}
%\def\endtable{\endcenter\endotable}
%\@namedef{table*}{\@dblfloat{table}[tp]\center\vskip-1mm\setlength{\nhsize}{\hsize}\addtolength{\nhsize}{-2\fboxsep}\addtolength{\nhsize}{-2\fboxrule}}
%\@namedef{endtable*}{\endcenter\end@dblfloat}

\long\def\@makecaption#1#2{%caption without box
  \def\@tempa{figure}\ifx\@captype\@tempa\vskip0.7\abovecaptionskip\fi%le-tex, 2012-03-16; 
                                               %reduction of skip is explicitly required (70% chosen by le-tex)
  {\reset@font\small{\bf #1.} #2\par}%
  \def\@tempa{table}\ifx\@captype\@tempa\vskip\abovecaptionskip\fi}%le-tex, 2012-03-16
%\long\def\@makecaption#1#2{\vskip 10\p@{\reset@font\small{\bf #1.} #2\par}\vskip 10\p@} % caption without box, use only with above figure/table
%\long\def\@makecaption#1#2{\vskip 10\p@\fcolorbox{bgcol}{bgcol}{\parbox{\nhsize}{\reset@font\small{\bf #1.} #2}\par}\vskip 10\p@} % caption with box

\let\@oxfloat=\@xfloat
\def\@xfloat#1[#2]{%
  \@oxfloat#1[#2]%
  \vspace*{2mm}%le-tex, 2012-03-26
  \centering%le-tex, 2012-03-16
  \small}

\sloppy\clubpenalty10000\widowpenalty10000\frenchspacing
\leftmargini 2em
\leftmarginii 1.75em
\leftmarginiii 1.5em
\leftmarginiv 1em
\leftmarginv .5em
\leftmarginvi .5em
\def\labelitemi{\bf --}
\def\labelitemii{\bf --}
\def\labelitemiii{$\m@th\bullet$}
\def\labelitemiv{$\m@th\cdot$}

\def\maketitle{
\def\supplement##1{\expandafter
   {\csname url\endcsname{http://\@journalurl/\@pvol/\@fpage/\@pyear/\@journalnameshortlower-\@pvol-\@fpage-\@pyear-supplement.##1}}}
\ifodd\@fpage\else
   \ClassError{copernicus2}
   {Starting page must be odd!}{change the firstpage command}
\fi
   \setcounter{footnote}{0}
   \setcounter{page}{\@fpage}
   \thispagestyle{titlepage}
   \setcounter{footnote}{0}%
   \twocolumn[%
      \vspace{36mm}\hspace{10mm}{\begin{minipage}[t]{157mm}
         \noindent\begin{minipage}{\hsize}
            \raggedright
            \let\footnotesize\normalsize
            \let\footnoterule\relax
            \def\thefootnote{\fnsymbol{footnote}}%
            \def\@mpfn{footnote}%
            \let\thempfn=\thefootnote
            \renewcommand\@makefntext[1]{%
            \parindent 1em \noindent
            \@makefnmark##1}
            {\sffamily\LARGE\bfseries
             \mathversion{bold}%le-tex, 2012-03-16
             \center\color{textcol}{\@title}\par}
            \vspace{10pt}
            {\normalsize
               \renewcommand{\Authfont}{\bfseries}%
               \renewcommand{\Affilfont}{\reset@font}%
               \setlength{\affilsep}{4pt}%
               \baselineskip 15pt
               \let\and=\sw@and
               \if@noauthor\else
                  \begin{minipage}[t]{\hsize}\flushleft\baselineskip=12pt%
                     \center\@author
                  \end{minipage}
               \fi
            \par}%
            \if!\@corresp!\else\center{\textit{Correspondence to:}} \@corresp\fi
         \end{minipage}
         \if!\@recvd!\else
            \if@twostagejnl
               \par\center{\small Received: \@recvd\ -- Published in \@journalnameabbreviation\ Discuss.: \@pubdiscuss\\[-0.5mm]
               Revised: \@revsd\ -- Accepted: \@accptd\ -- Published: \@published}
            \else         
               \par\center{\small Received: \@recvd\ \if!\@revsd!\else -- Revised: \@revsd\ \fi
               \if!\@accptd!\else -- Accepted: \@accptd\ \fi -- Published: \@published}
            \fi
         \fi
         \if!\@abstr!\else
            \par\vspace{10pt}
            \setlength{\nhsize}{\hsize}\addtolength{\nhsize}{-2\fboxsep}\addtolength{\nhsize}{-2\fboxrule}         
            \fcolorbox{bgcol}{bgcol}{\parbox{\nhsize}{%
               \par\noindent{\reset@font\normalsize\sffamily\bfseries\color{textcol}\abstractname.}\enspace\ignorespaces\@abstr % abstract name left
               %{\vspace{-7.5mm}\center\reset@font\normalsize\sffamily\bfseries\color{textcol}\center\abstractname\\[1.7mm]}\@abstr % abstract name centered
               \if!\@keyw!\else\vspace{1.7mm}\par\noindent{\reset@font\normalsize\sffamily\bfseries\color{textcol} Keywords.}\enspace\ignorespaces\@keyw\fi
            }}
         \fi
         \par\vspace{20pt}
      \end{minipage}%
   }]%
   {\renewcommand\@makefntext[1]{\parindent 1em \noindent ##1}%
   \let\footnotesize\normalsize
%%CC and Correspondence in footer removed! (Dec 2011) %%   
%   \if@cop@home
%      \footnotetext{\vskip0mm\hskip-3mm %
%      \if@nohyperref
%         \includegraphics[width=2.5cm]{CreativeCommons_Attribution_License.png}
%      \else
%         \href{http://creativecommons.org/licenses/by/3.0/}{\includegraphics[width=2.5cm]{CreativeCommons_Attribution_License.png}}
%      \fi %
%      \if!\@corresp!\else\hskip2mm\parbox{57mm}{\vskip-6mm\textit{Correspondence to:} \@corresp}\fi}
%   \else
%      \if!\@corresp!\else\footnotetext{\textit{Correspondence to:} \@corresp}\fi
%   \fi
   \let\footnotesize\small
   \setcounter{footnote}{0}}%
}

\renewcommand\and{\@centercr}
\newcommand\sw@and{\end{minipage}\\[1em]\begin{minipage}[t]{\hsize}%
   \flushleft\baselineskip12pt}
   
\def\bartext#1{\def\@btext{#1}}                        \def\@btext{}

\def\pubyear#1{\if@cop@home\def\@pyear{#1}\fi}         \def\@pyear{\number\year}
\def\pubvol#1{\if@cop@home\def\@pvol{#1}\fi}           \def\@pvol{1}
 \ifwe
 \def\pubvol#1{\if@cop@home\def\@pvol{#1}\fi}           \def\@pvol{11}
 \fi
\def\firstpage#1{\if@cop@home\def\@fpage{#1}\fi}       \def\@fpage{1}

\def\runningauthor#1{\def\@runauth{#1}}                \def\@runauth{}
\def\runningtitle#1{\def\@runtit{#1}}                  \def\@runtit{}
\def\runninghead#1{\def\@runhd{#1}}                    
\def\@runhd{
  \ifx\firstrunhd@utput\@undefined%le-tex, 2012-06-18
    \global\let\firstrunhd@utput\relax
  \else
    \def\blackb@x[##1]##2{\relax}%
  \fi
  \@runauth: \@runtit}

\def\correspondence#1{\def\@corresp{#1}}               \def\@corresp{}

\def\pubdiscuss#1{\if@cop@home\def\@pubdiscuss{#1}\fi} \def\@pubdiscuss{}
\def\received#1{\if@cop@home\def\@recvd{#1}\fi}        \def\@recvd{}
\def\revised#1{\if@cop@home\def\@revsd{#1}\fi}         \def\@revsd{}
\def\accepted#1{\if@cop@home\def\@accptd{#1}\fi}       \def\@accptd{}
\def\published#1{\if@cop@home\def\@published{#1}\fi}   \def\@published{}

\def\lastpage@putlabel{\addtocounter{page}{-1}%
   \immediate\write\@auxout{\string
   \newlabel{LastPage}{{}{\thepage}{}{}{}}}%
   \addtocounter{page}{1}}
\AtEndDocument{\clearpage\lastpage@putlabel}
\def\@lpage{\hypersetup{linkcolor=textcol}\pageref{LastPage}}

\def\keywords#1{\def\@keyw{#1}}                        \def\@keyw{}
\def\abstract#1{\def\@abstr{#1}}                       \def\@abstr{}
   
\newenvironment{acknowledgements}{\vspace{\baselineskip}\begin{description}\item[\color{textcol}\acknowsname]}{\end{description}}
\newenvironment{acknowledgement}{\vspace{\baselineskip}\begin{description}\item[\color{textcol}\acknowname]}{\end{description}}
   
\def\ps@runninghead{\let\@mkboth\@gobbletwo
   \if@cop@home
      \def\@oddhead{{\reset@font\sffamily\normalsize\color{textcol}\@runhd\hfil\makebox[\z@][r]{\thepage}
         \if@bar   
            {\setlength{\unitlength}{1.0cm}
            \begin{picture}(0,0)
            \color{barcol}\put(0.7,0){\rule[-300mm]{14mm}{400mm}}
            \color{textcol}\put(0.7,0){\rule[-300mm]{0.8pt}{400mm}}
            \end{picture}}
         \fi}}
      \def\@evenhead{{\reset@font\sffamily\normalsize\color{textcol}\makebox[\z@][l]{\thepage}\hfil\@runhd
         \if@bar
            {\setlength{\unitlength}{1.0cm}
            \begin{picture}(0,0)
            \color{barcol}\put(-19.7,0){\rule[-300mm]{14mm}{400mm}}
            \color{textcol}\put(-18.3,0){\rule[-300mm]{0.8pt}{400mm}}
            \end{picture}}
         \fi}}
      \ifcopyediting
      \def\@oddfoot{\edit@rnotereminder\reset@font\sffamily\normalsize\hfil \textcolor[RGB]{255,149,64}{Copy-Editing Track Changes Document -- No Source for Author's Proof-Reading}}
      \else
      \def\@oddfoot{\edit@rnotereminder{\reset@font\sffamily\normalsize\color{textcol}\@journalurl\if@noref\else/\@pvol/\@fpage/\@pyear/\fi
         \hfil \@journalnameabbreviation\if@noref\else, \@pvol, \@fpage\if@nolastpage\else{--\@lpage}\fi, \@pyear\fi}}
      \fi
      \ifcopyediting
      \def\@evenfoot{\edit@rnotereminder\reset@font\sffamily\normalsize \textcolor[RGB]{255,149,64}{Copy-Editing Track Changes Document -- No Source for Author's Proof-Reading} \hfil}
      \else
      \def\@evenfoot{\edit@rnotereminder{\reset@font\sffamily\normalsize\color{textcol}\@journalnameabbreviation\if@noref\else, \@pvol, \@fpage\if@nolastpage\else{--\@lpage}\fi, \@pyear\fi
         \hfil\@journalurl\if@noref\else/\@pvol/\@fpage/\@pyear/\fi}}
      \fi
   \else
      \def\@oddhead{\@runhd\hfil\makebox[\z@][r]{\thepage}}
      \def\@evenhead{\makebox[\z@][l]{\thepage}\hfil\@runhd}
      \def\@oddfoot{\@journalurl\hfil\@journalnameabbreviation}
      \def\@evenfoot{\@journalnameabbreviation\hfil\@journalurl}
   \fi}
   
\def\ps@titlepage{\let\@mkboth\@gobbletwo
   \if@cop@home
      \ifcopyediting
      \def\@oddhead{{\reset@font\sffamily\normalsize\parbox[t]{\textwidth}{
      \textcolor[RGB]{255,149,64}{Copy-Editing Track Changes Document -- No Source for Author's Proof-Reading}\\
      \color[gray]{0.55}\put(0.7,0){\rule[1mm]{\textwidth}{0.1mm}}\\
      \textcolor[gray]{0.55}{Your manuscript was typeset and received English language copy-editing.\\
      The resulting file is available for proof-reading in the final journal style.\\
      The present document is only to review the language changes.}}
      \if@bar
            {\setlength{\unitlength}{1.0cm}
            \begin{picture}(0,0)
            \color{barcol}\put(0.7,0){\rule[-300mm]{14mm}{400mm}} 
            \color{textcol}\put(0.7,0){\rule[-300mm]{0.8pt}{400mm}}           
            \put(1,0.3){\rotatebox{270}{\reset@font\sffamily\Large\color{textcol}\@btext}}      
            \end{picture}}
         \fi}}
      \else
      \def\@oddhead{{\reset@font\sffamily\normalsize\color{textcol}\parbox[t]{0.5\textwidth}{%
         \@journalnameabbreviation\if@noref\else, \@pvol, \@fpage\if@nolastpage\else{--\@lpage}\fi, \@pyear\fi\\
         \@journalurl\if@noref\else/\@pvol/\@fpage/\@pyear/\fi\\
         \if@noref\else doi:10.5194/\@journalnameshortlower-\@pvol-\@fpage-\@pyear\fi\\
         \@journalcopyright}
         \hfill\@journallogo
         \if@bar
            {\setlength{\unitlength}{1.0cm}
            \begin{picture}(0,0)
            \color{barcol}\put(0.7,0){\rule[-300mm]{14mm}{400mm}} 
            \color{textcol}\put(0.7,0){\rule[-300mm]{0.8pt}{400mm}}           
            \put(1,0.3){\rotatebox{270}{\reset@font\sffamily\Large\color{textcol}\@btext}}      
            \end{picture}}
         \fi}}             
      \fi
      \def\@oddfoot{\edit@rnotereminder{\reset@font\sffamily\normalsize\color{textcol}\@sentence\hfil}}
   \else
      \def\@oddhead{\parbox[t]{0.6\textwidth}{%
         Manuscript prepared for \@journalnameabbreviation\\
         with version \clsversion\ of the \LaTeX\ class copernicus2.cls.\\
         Date: \number\day~\ifcase\month\or January\or February\or%
            March\or April\or May\or June\or July\or August\or September\or%
            October\or November\or December\fi~\number\year}}
      \def\@oddfoot{}
   \fi}
   
\pagenumbering{arabic}

\flushbottom
\ps@runninghead

\AtBeginDocument{\mathindent\z@}
%% commands for narrow math
\AtBeginDocument{%
  \thinmuskip=2mu
  \medmuskip=3mu minus 3mu
  \thickmuskip=4mu}

%>le-tex, 2012-03-16
\def\Supplementary#1{{%
\advance\partopsep\baselineskip
  \begin{trivlist}\bfseries\raggedright
  \item Supplementary material related to this article is available online at:
    \supplement{#1}.
  \end{trivlist}}}
%>le-tex, 2012-04-18
\newdimen\tabularwidth
\def\@tabular{%
  \leavevmode
  \setbox\z@
  \hbox \bgroup $\col@sep\tabcolsep \let\d@llarbegin\begingroup
                                    \let\d@llarend\endgroup
  \@tabarray}
\def\endtabular{\endarray $\egroup
  \global\tabularwidth\wd\z@
  \unhbox\z@}
\expandafter\let\csname endtabular*\endcsname=\endtabular
%<le-tex, 2012-04-18
\DeclareRobustCommand\belowtable[1]{%
  \par\vspace{1ex}%
  \ifdim\tabularwidth<10mm\tabularwidth\hsize\fi
  \parbox{\tabularwidth}{\raggedright\scriptsize#1}}
%<le-tex, 2012-03-16

%DOI handling: (le-tex, 2012-03-16)
%Thanks to doi.sty[2007/07/24 handle doi numbers] which is not available in older TeX distributions
\newcommand{\doitext}{doi:}
\newcommand*{\doi}{% 
  \begingroup 
  \lccode`\~=`\#\relax 
  \lowercase{\def~{\#}}%
  \lccode`\~=`\_\relax
  \lowercase{\def~{\_}}%
  \lccode`\~=`\<\relax 
  \lowercase{\def~{\textless}}%
  \lccode`\~=`\>\relax 
  \lowercase{\def~{\textgreater}}%
  \lccode`\~=0\relax
  \catcode`\#=\active 
  \catcode`\_=\active 
  \catcode`\<=\active 
  \catcode`\>=\active
  %catcode change won't work after \bibitem while backref.sty
  %  grabs for the whole entry
  \@doi}
\def\@doi#1{% 
  \let\#\relax
  \let\_\relax
  \let\textless\relax 
  \let\textgreater\relax 
  \edef\x{\toks0={{#1}}}% 
  \x
  \edef\#{\@percentchar23}%
  \edef\_{_}%
  \edef\textless{\@percentchar3C}% instead of {\string<} for Apple
  \edef\textgreater{\@percentchar3E}% instead of {\sting>} for Apple
  \edef\x{\toks1={\noexpand\href{http://dx.doi.org/#1}}}% 
  \x
  \edef\x{\endgroup\doitext\the\toks1 \the\toks0}%
  \x}

%Copy-editor and Typesetter notes: (le-tex, 2012-03-16)
\newcounter{CEnote}
\newcounter{TSnote}
\def\theCEnote{\@arabic\c@CEnote}
\def\theTSnote{\@arabic\c@TSnote}
\newtoks\CEnotes
\newtoks\TSnotes
\global\let\edit@rnotereminder\@empty%initialising
\def\n@telabel#1{\noexpand\n@tel@bel{#1\csname the#1note\endcsname}}
\def\n@tel@bel#1{%
  \smash{\raisebox{0.2ex}{\bfseries\scriptsize\fboxsep1pt\colorbox[RGB]{255,149,64}{\textcolor[RGB]{255,255,255}{#1}}}}}
\def\n@tel@bel#1{%
  \def\rlap##1{\hb@xt@ \z@ {##1\hss }}
  \smash{\raisebox{0.2ex}{\bfseries\scriptsize\fboxsep1pt\colorbox[RGB]{255,149,64}{\textcolor[RGB]{255,255,255}{#1}}}}}

\DeclareRobustCommand\blackbox{\@ifnextchar[\blackb@x{\blackb@x[TS]}}%]
\def\blackb@x[#1]#2{%
  \refstepcounter{#1note}%
  \ClassWarningNoLine{copernicus2}{Editor's note on page \thepage.}%
  \hypertarget{#1\csname the#1note\endcsname}{\hyperlink{#1\csname the#1note\endcsname R}{\n@tel@bel{#1\csname the#1note\endcsname}}}%
  \gdef\edit@rnotereminder{%
    \smash{\llap{\rotatebox{90}{\lower1ex\rlap{\fboxsep0.5ex\colorbox[RGB]{255,149,64}{\bfseries\scriptsize\strut\textcolor[RGB]{255,255,255}{Please note the remarks at the end of the manuscript.}}}}\kern\marginparsep}}%
    \global\let\edit@rnotereminder\@empty}%
  \edef\@tempa{\leavevmode\noexpand\hypertarget{#1\csname the#1note\endcsname R}{\noexpand\hyperlink{#1\csname the#1note\endcsname}{\noexpand\hb@xt@10mm{\n@telabel{#1}\hss}}}}%
  \global\csname #1notes\endcsname\expandafter\expandafter\expandafter{\the\expandafter\csname #1notes\expandafter\endcsname\@tempa\mdseries#2\par}}
\AtEndDocument{%
  \onecolumn
  \thispagestyle{empty}%
  \parindent\z@
  \expandafter\def\expandafter\@tempa\expandafter{\the\CEnotes}%
  \expandafter\def\expandafter\@tempb\expandafter{\the\TSnotes}%
  \ifx\@tempa\@empty
  \else
    \@tempcnta-\c@page
    \section*{Remarks from the English Copy-Editor}
    \hangindent10mm
    \@tempa
  \fi
  \ifx\@tempb\@empty
    \ifx\@tempa\@empty\else\advance\@tempcnta\c@page\advance\@tempcnta\@ne\ClassInfo{copernicus2}{Pages with editing remarks: \the\@tempcnta.}\fi
  \else
    \ifx\@tempa\@empty\@tempcnta-\c@page\fi
    \section*{Remarks from the Typesetter}
    \hangindent10mm
    \@tempb
    \advance\@tempcnta\c@page\advance\@tempcnta\@ne\ClassInfo{copernicus2}{Pages with editing remarks: \the\@tempcnta.}
  \fi}

%Formatting helper (le-tex, 2012-03-16)
\let\hack\@firstofone

\endinput
%% End of file `copernicus2.cls'.
