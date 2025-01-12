#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <fcntl.h>
#include <io.h>

#include "db/Database.h"
#include "network/Client.h"
#include "view/Application.h"
#include "db/Parser.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    RECT rc = { 200, 200, 1800, 1200 };
    Application app = Application(rc);

    // Client::Fetch("9784764904057");

    std::string xml_docs =  R"(
<searchRetrieveResponse xmlns="http://www.loc.gov/zing/srw/">
  <version>1.2</version>
  <numberOfRecords>1</numberOfRecords>
  <extraResponseData>
    <facets>
      <lst name="REPOSITORY_NO">
        <int name="R100000002">1</int>
      </lst>
      <lst name="NDC">
        <int name="4">1</int>
      </lst>
      <lst name="ISSUED_DATE">
        <int name="2015">1</int>
      </lst>
      <lst name="LIBRARY">
        <int name="千葉県立西部図書館">1</int>
        <int name="国立国会図書館">1</int>
        <int name="広島県立図書館">1</int>
        <int name="愛知県図書館">1</int>
        <int name="東京都立中央図書館">1</int>
        <int name="横浜市立図書館">1</int>
        <int name="神奈川県立川崎図書館">1</int>
        <int name="福井県立図書館">1</int>
      </lst>
    </facets>
  </extraResponseData>
  <records>
    <record>
      <recordSchema>info:srw/schema/1/dc-v1.1</recordSchema>
      <recordPacking>xml</recordPacking>
      <recordData>
        <rdf:RDF xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:rdfs="http://www.w3.org/2000/01/rdf-schema#" xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:dcterms="http://purl.org/dc/terms/" xmlns:dcndl="http://ndl.go.jp/dcndl/terms/" xmlns:foaf="http://xmlns.com/foaf/0.1/" xmlns:owl="http://www.w3.org/2002/07/owl#">
          <dcndl:BibAdminResource rdf:about="https://ndlsearch.ndl.go.jp/books/R100000002-I026975592">
            <dcndl:catalogingStatus>C7</dcndl:catalogingStatus>
            <dcndl:catalogingRule>ncr/1987</dcndl:catalogingRule>
            <dcterms:description>type : book</dcterms:description>
            <dcndl:bibRecordCategory>R100000002</dcndl:bibRecordCategory>
            <dcndl:bibRecordSubCategory>111</dcndl:bibRecordSubCategory>
            <dcndl:record rdf:resource="https://ndlsearch.ndl.go.jp/books/R100000002-I026975592#material"/>
          </dcndl:BibAdminResource>
          <dcndl:BibResource rdf:about="https://ndlsearch.ndl.go.jp/books/R100000002-I026975592#material">
            <rdfs:seeAlso rdf:resource="http://id.ndl.go.jp/jpno/22674404"/>
            <rdfs:seeAlso rdf:resource="http://id.ndl.go.jp/bib/026975592"/>
            <dcterms:identifier rdf:datatype="http://ndl.go.jp/dcndl/terms/JPNO">22674404</dcterms:identifier>
            <dcterms:identifier rdf:datatype="http://ndl.go.jp/dcndl/terms/NDLBibID">026975592</dcterms:identifier>
            <dcterms:identifier rdf:datatype="http://ndl.go.jp/dcndl/terms/TOHANMARCNO">33378452</dcterms:identifier>
            <rdfs:seeAlso rdf:resource="http://iss.ndl.go.jp/isbn/9784764904057"/>
            <dcterms:identifier rdf:datatype="http://ndl.go.jp/dcndl/terms/ISBN">978-4-7649-0405-7</dcterms:identifier>
            <dcterms:title>ストラング:線形代数イントロダクション</dcterms:title>
            <dc:title>
              <rdf:Description>
                <rdf:value>ストラング:線形代数イントロダクション</rdf:value>
                <dcndl:transcription>ストラング センケイ ダイスウ イントロダクション</dcndl:transcription>
              </rdf:Description>
            </dc:title>
            <dcndl:alternative>
              <rdf:Description>
                <rdf:value>Introduction to Linear Algebra</rdf:value>
              </rdf:Description>
            </dcndl:alternative>
            <dcndl:seriesTitle>
              <rdf:Description>
                <rdf:value>世界標準MIT教科書</rdf:value>
                <dcndl:transcription>セカイ ヒョウジュン エムアイティー キョウカショ</dcndl:transcription>
              </rdf:Description>
            </dcndl:seriesTitle>
            <dcterms:creator>
              <foaf:Agent rdf:about="http://id.ndl.go.jp/auth/entity/00457883">
                <foaf:name>Strang, Gilbert</foaf:name>
              </foaf:Agent>
            </dcterms:creator>
            <dcterms:creator>
              <foaf:Agent rdf:about="http://id.ndl.go.jp/auth/entity/01219758">
                <foaf:name>松崎, 公紀</foaf:name>
                <dcndl:transcription>マツザキ, キミノリ</dcndl:transcription>
              </foaf:Agent>
            </dcterms:creator>
            <dcterms:creator>
              <foaf:Agent rdf:about="http://id.ndl.go.jp/auth/entity/00720894">
                <foaf:name>新妻, 弘, 1946-</foaf:name>
                <dcndl:transcription>ニイツマ, ヒロシ, 1946-</dcndl:transcription>
              </foaf:Agent>
            </dcterms:creator>
            <dc:creator>ギルバート・ストラング 著</dc:creator>
            <dc:creator>松崎公紀, 新妻弘 共訳</dc:creator>
            <dcterms:publisher>
              <foaf:Agent>
                <foaf:name>近代科学社</foaf:name>
                <dcndl:transcription>キンダイカガクシャ</dcndl:transcription>
                <dcndl:location>東京</dcndl:location>
              </foaf:Agent>
            </dcterms:publisher>
            <dcndl:publicationPlace rdf:datatype="http://purl.org/dc/terms/ISO3166">JP</dcndl:publicationPlace>
            <dcterms:date>2015.12</dcterms:date>
            <dcterms:issued rdf:datatype="http://purl.org/dc/terms/W3CDTF">2015</dcterms:issued>
            <dcterms:description>原タイトル: Introduction to Linear Algebra 原著第4版の翻訳</dcterms:description>
            <dcterms:description>文献あり 索引あり</dcterms:description>
            <dcterms:subject>
              <rdf:Description rdf:about="http://id.ndl.go.jp/auth/ndlsh/00570681">
                <rdf:value>線型代数学</rdf:value>
                <dcndl:transcription>センケイダイスウガク</dcndl:transcription>
              </rdf:Description>
            </dcterms:subject>
            <dcterms:subject rdf:resource="http://id.ndl.go.jp/class/ndlc/MA64"/>
            <dcterms:subject rdf:resource="http://id.ndl.go.jp/class/ndc9/411.3"/>
            <dcterms:language rdf:datatype="http://purl.org/dc/terms/ISO639-2">jpn</dcterms:language>
            <dcndl:originalLanguage rdf:datatype="http://purl.org/dc/terms/ISO639-2">eng</dcndl:originalLanguage>
            <dcndl:price>8000円</dcndl:price>
            <dcterms:extent>625p ; 27cm</dcterms:extent>
            <dcndl:materialType rdf:resource="http://ndl.go.jp/ndltype/Book" rdfs:label="図書"/>
            <dcterms:audience>一般</dcterms:audience>
            <rdfs:seeAlso rdf:resource="https://ndlsearch.ndl.go.jp/books/R100000002-I026975592"/>
          </dcndl:BibResource>
          <dcndl:BibResource rdf:about="https://ndlsearch.ndl.go.jp/books/R100000002-I026975592#material">
            <dcndl:record rdf:resource="https://ndlsearch.ndl.go.jp/books/R100000001-I12121000100344833#item"/>
            <dcndl:record rdf:resource="https://ndlsearch.ndl.go.jp/books/R100000001-I13111152746143#item"/>
            <dcndl:record rdf:resource="https://ndlsearch.ndl.go.jp/books/R100000001-I14121105333533#item"/>
            <dcndl:record rdf:resource="https://ndlsearch.ndl.go.jp/books/R100000001-I14211113334228#item"/>
            <dcndl:record rdf:resource="https://ndlsearch.ndl.go.jp/books/R100000001-I18111110150996#item"/>
            <dcndl:record rdf:resource="https://ndlsearch.ndl.go.jp/books/R100000001-I23111190184416#item"/>
            <dcndl:record rdf:resource="https://ndlsearch.ndl.go.jp/books/R100000001-I3411B12245048#item"/>
            <dcndl:record rdf:resource="https://ndlsearch.ndl.go.jp/books/R100000002-I026975592#item"/>
            <dcndl:record rdf:resource="https://ndlsearch.ndl.go.jp/books/R100000136-I1130000796644830464#item"/>
            <dcndl:record rdf:resource="https://ndlsearch.ndl.go.jp/books/R100000137-I7649040511000000000r#item"/>
            <dcndl:record rdf:resource="https://ndlsearch.ndl.go.jp/books/R100000137-I9784764904057#item"/>
          </dcndl:BibResource>
        </rdf:RDF>
      </recordData>
      <recordPosition>1</recordPosition>
    </record>
  </records>
</searchRetrieveResponse>
    )";

    Parser::Initialize();
    Book b = Parser::Parse(xml_docs);
    Parser::Terminate();

    Database::Connect();
    Database::Insert(b);
    Database::Disconnect();

    // app.Init();
    //
    // ShowWindow(app.Window(), nCmdShow);
    // UpdateWindow(app.Window());
    //
    // MSG msg = {};
    // while (GetMessage(&msg, nullptr, 0, 0))
    // {
    //     TranslateMessage(&msg);
    //     DispatchMessage(&msg);
    // }

    return 0;
}
