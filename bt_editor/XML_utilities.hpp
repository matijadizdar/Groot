#ifndef XMLPARSERS_HPP
#define XMLPARSERS_HPP

#include <QDomDocument>
#include <QXmlStreamWriter>
#include "bt_editor_base.h"

#include <nodes/Node>
#include <nodes/NodeData>
#include <nodes/FlowScene>
#include <nodes/DataModelRegistry>


NodeModels ReadTreeNodesModel(const QDomElement& root);

void RecursivelyCreateXml(const QtNodes::FlowScene &scene,
                          QDomDocument& doc,
                          QDomElement& parent_element,
                          const QtNodes::Node* node);

bool VerifyXML(QDomDocument& doc,
               const std::vector<QString> &registered_ID,
               std::vector<QString> &error_messages);

NodeModel buildTreeNodeModelFromXML(const QDomElement &node);

QDomElement writePortModel(const QString &port_name, const PortModel &port, QDomDocument &doc);

QString xmlDocumentToString(const QDomDocument &document);

void streamElementAttributes(QXmlStreamWriter &stream, const QDomElement &element);

void recursivelySaveNodeCanonically(QXmlStreamWriter &stream, const QDomNode &parent_node);

#endif // XMLPARSERS_HPP
