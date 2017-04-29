# Better generation

Why do this at runtime and not as a compiler plugin running over the AST ?
I don't think there is a good reason for it.


Here is some code to generate type output from the AST (from clang), looks
fairly easy.

```
void DeclPrinter::PrintObjCMethodType(ASTContext &Ctx,
Decl::ObjCDeclQualifier Quals,
QualType T) {
if (Quals & Decl::ObjCDeclQualifier::OBJC_TQ_In)
Out << "in ";
if (Quals & Decl::ObjCDeclQualifier::OBJC_TQ_Inout)
Out << "inout ";
if (Quals & Decl::ObjCDeclQualifier::OBJC_TQ_Out)
Out << "out ";
if (Quals & Decl::ObjCDeclQualifier::OBJC_TQ_Bycopy)
Out << "bycopy ";
if (Quals & Decl::ObjCDeclQualifier::OBJC_TQ_Byref)
Out << "byref ";
if (Quals & Decl::ObjCDeclQualifier::OBJC_TQ_Oneway)
Out << "oneway ";
if (Quals & Decl::ObjCDeclQualifier::OBJC_TQ_CSNullability) {
if (auto nullability = AttributedType::stripOuterNullability(T))
Out << getNullabilitySpelling(*nullability, true) << ' ';
}

Out << Ctx.getUnqualifiedObjCPointerType(T).getAsString(Policy);
}


void DeclPrinter::VisitObjCMethodDecl(ObjCMethodDecl *OMD) {

if (!OMD->getReturnType().isNull()) {
PrintObjCMethodType(OMD->getASTContext(), OMD->getObjCDeclQualifier(),
OMD->getReturnType());
}
}

void DeclPrinter::VisitObjCMethodDecl(ObjCMethodDecl *OMD) {
{
std::string name = OMD->getSelector().getAsString();
std::string::size_type pos, lastPos = 0;
for (const auto *PI : OMD->parameters()) {
// FIXME: selector is missing here!
pos = name.find_first_of(':', lastPos);
PrintObjCMethodType(OMD->getASTContext(),
PI->getObjCDeclQualifier(),
PI->getType());
Out << *PI;
lastPos = pos + 1;
}
}

void DeclPrinter::VisitObjCMethodDecl(ObjCMethodDecl *OMD) {
{
prettyPrintAttributes(OMD);
}
```
