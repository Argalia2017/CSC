﻿#pragma once

#include "csc.hpp"
#include "csc_core.hpp"
#include "csc_basic.hpp"
#include "csc_array.hpp"
#include "csc_math.hpp"

#include "begin.h"
#include <cmath>
#include "end.h"

namespace CSC {
namespace MATH {
template <class...>
trait MATHPROC_IMPLHOLDER_HELP ;

template <>
trait MATHPROC_IMPLHOLDER_HELP<ALWAYS> {
	using Holder = typename MATHPROC_HELP<ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	public:
		implicit ImplHolder () = default ;

		BOOL is_infinity (CREF<SINGLE> x) const override {
			const auto r1x = bitwise (x) ;
			if ((r1x & CHAR (0X7F800000)) != CHAR (0X7F800000))
				return FALSE ;
			//@warn: nan is a deprecated magic number
			if ((r1x & CHAR (0X007FFFFF)) != CHAR (0X00))
				return TRUE ;
			return TRUE ;
		}

		BOOL is_infinity (CREF<DOUBLE> x) const override {
			const auto r1x = bitwise (x) ;
			if ((r1x & DATA (0X7FF0000000000000)) != DATA (0X7FF0000000000000))
				return FALSE ;
			//@warn: nan is a deprecated magic number
			if ((r1x & DATA (0X000FFFFFFFFFFFFF)) != DATA (0X00))
				return TRUE ;
			return TRUE ;
		}

		SINGLE sign (CREF<SINGLE> x) const override {
			const auto r1x = bitwise (x) ;
			if ((r1x & CHAR (0X80000000)) == CHAR (0X00))
				return SINGLE (1) ;
			return SINGLE (-1) ;
		}

		DOUBLE sign (CREF<DOUBLE> x) const override {
			const auto r1x = bitwise (x) ;
			if ((r1x & DATA (0X8000000000000000)) == DATA (0X00))
				return DOUBLE (1) ;
			return DOUBLE (-1) ;
		}

		SINGLE step (CREF<SINGLE> x) const override {
			const auto r1x = bitwise (x) ;
			if ((r1x & CHAR (0X80000000)) == CHAR (0X00))
				return SINGLE (1) ;
			return SINGLE (0) ;
		}

		DOUBLE step (CREF<DOUBLE> x) const override {
			const auto r1x = bitwise (x) ;
			if ((r1x & DATA (0X8000000000000000)) == DATA (0X00))
				return DOUBLE (1) ;
			return DOUBLE (0) ;
		}

		SINGLE abs (CREF<SINGLE> x) const override {
			const auto r1x = bitwise (x) ;
			if ((r1x & CHAR (0X80000000)) == CHAR (0X00))
				return x ;
			const auto r2x = r1x & ~CHAR (0X80000000) ;
			return bitwise (TYPEAS<SINGLE>::id ,r2x) ;
		}

		DOUBLE abs (CREF<DOUBLE> x) const override {
			const auto r1x = bitwise (x) ;
			if ((r1x & DATA (0X8000000000000000)) == DATA (0X00))
				return x ;
			const auto r2x = r1x & ~DATA (0X8000000000000000) ;
			return bitwise (TYPEAS<DOUBLE>::id ,r2x) ;
		}

		SINGLE inverse (CREF<SINGLE> x) const override {
			if (abs (x) < SINGLE_EPS)
				return SINGLE (0) ;
			return SINGLE (1) / x ;
		}

		DOUBLE inverse (CREF<DOUBLE> x) const override {
			if (abs (x) < DOUBLE_EPS)
				return DOUBLE (0) ;
			return DOUBLE (1) / x ;
		}

		SINGLE floor (CREF<SINGLE> x ,CREF<SINGLE> y) const override {
			assert (y > 0) ;
			const auto r1x = VAL64 (x * MathProc::inverse (y)) ;
			const auto r2x = y * SINGLE (r1x) ;
			if (x >= 0)
				return r2x ;
			if (x >= r2x)
				return r2x ;
			return y * SINGLE (r1x - 1) ;
		}

		DOUBLE floor (CREF<DOUBLE> x ,CREF<DOUBLE> y) const override {
			assert (y > 0) ;
			const auto r1x = VAL64 (x * MathProc::inverse (y)) ;
			const auto r2x = y * DOUBLE (r1x) ;
			if (x >= 0)
				return r2x ;
			if (x >= r2x)
				return r2x ;
			return y * DOUBLE (r1x - 1) ;
		}

		SINGLE ceil (CREF<SINGLE> x ,CREF<SINGLE> y) const override {
			assert (y > 0) ;
			const auto r1x = VAL64 (x * MathProc::inverse (y)) ;
			const auto r2x = y * SINGLE (r1x) ;
			if (x <= 0)
				return r2x ;
			if (x <= r2x)
				return r2x ;
			return y * SINGLE (r1x + 1) ;
		}

		DOUBLE ceil (CREF<DOUBLE> x ,CREF<DOUBLE> y) const override {
			assert (y > 0) ;
			const auto r1x = VAL64 (x * MathProc::inverse (y)) ;
			const auto r2x = y * DOUBLE (r1x) ;
			if (x <= 0)
				return r2x ;
			if (x <= r2x)
				return r2x ;
			return y * DOUBLE (r1x + 1) ;
		}

		DOUBLE sqrt (CREF<DOUBLE> x) const override {
			return std::sqrt (x) ;
		}

		DOUBLE cbrt (CREF<DOUBLE> x) const override {
			return std::cbrt (x) ;
		}

		DOUBLE exp (CREF<DOUBLE> x) const override {
			return std::exp (x) ;
		}

		DOUBLE log (CREF<DOUBLE> x) const override {
			return std::log (x) ;
		}

		DOUBLE pow (CREF<DOUBLE> x ,CREF<DOUBLE> y) const override {
			return std::pow (x ,y) ;
		}

		DOUBLE cndf (CREF<DOUBLE> x) const override {
			const auto r1x = x * MathProc::inverse (DOUBLE (MATH_SQRT2)) ;
			return std::erf (r1x) * DOUBLE (MATH_INV2) + DOUBLE (MATH_INV2) ;
		}

		DOUBLE pndf (CREF<DOUBLE> x) const override {
			const auto r1x = -MathProc::square (x) * DOUBLE (MATH_INV2) ;
			return exp (r1x) * DOUBLE (MATH_INVSQRT2PI) ;
		}

		DOUBLE sin (CREF<DOUBLE> x) const override {
			return std::sin (x) ;
		}

		DOUBLE cos (CREF<DOUBLE> x) const override {
			return std::cos (x) ;
		}

		DOUBLE tan (CREF<DOUBLE> x) const override {
			return std::tan (x) ;
		}

		DOUBLE arcsin (CREF<DOUBLE> x) const override {
			return std::asin (x) ;
		}

		DOUBLE arccos (CREF<DOUBLE> x) const override {
			return std::acos (x) ;
		}

		DOUBLE arctan (CREF<DOUBLE> x ,CREF<DOUBLE> y) const override {
			return std::atan2 (y ,x) ;
		}
	} ;
} ;

exports auto MATHPROC_HELP<ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename MATHPROC_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;

template <class...>
trait FLOATPROC_IMPLHOLDER_HELP ;

template <class...>
trait FLOATPROC_KROSHCACHE_HELP ;

template <>
trait FLOATPROC_KROSHCACHE_HELP<ALWAYS> {
	using NOTATION = typename FLOATPROC_HELP<ALWAYS>::NOTATION ;
	using SIZE = ENUMAS<VAL ,ENUMID<687>> ;
	using OFFSET = ENUMAS<VAL ,ENUMID<343>> ;

	class KroshCache {
	private:
		BoxBuffer<DATA ,SIZE> mMCache ;
		BoxBuffer<LENGTH ,SIZE> mECache ;

	public:
		imports CREF<KroshCache> instance () {
			return memorize ([&] () {
				KroshCache ret ;
				ret.mMCache = BoxBuffer<DATA ,SIZE> ({
					DATA (0XBF29DCABA82FDEAE) ,DATA (0XEEF453D6923BD65A) ,DATA (0X9558B4661B6565F8) ,
					DATA (0XBAAEE17FA23EBF76) ,DATA (0XE95A99DF8ACE6F54) ,DATA (0X91D8A02BB6C10594) ,
					DATA (0XB64EC836A47146FA) ,DATA (0XE3E27A444D8D98B8) ,DATA (0X8E6D8C6AB0787F73) ,
					DATA (0XB208EF855C969F50) ,DATA (0XDE8B2B66B3BC4724) ,DATA (0X8B16FB203055AC76) ,
					DATA (0XADDCB9E83C6B1794) ,DATA (0XD953E8624B85DD79) ,DATA (0X87D4713D6F33AA6C) ,
					DATA (0XA9C98D8CCB009506) ,DATA (0XD43BF0EFFDC0BA48) ,DATA (0X84A57695FE98746D) ,
					DATA (0XA5CED43B7E3E9188) ,DATA (0XCF42894A5DCE35EA) ,DATA (0X818995CE7AA0E1B2) ,
					DATA (0XA1EBFB4219491A1F) ,DATA (0XCA66FA129F9B60A7) ,DATA (0XFD00B897478238D1) ,
					DATA (0X9E20735E8CB16382) ,DATA (0XC5A890362FDDBC63) ,DATA (0XF712B443BBD52B7C) ,
					DATA (0X9A6BB0AA55653B2D) ,DATA (0XC1069CD4EABE89F9) ,DATA (0XF148440A256E2C77) ,
					DATA (0X96CD2A865764DBCA) ,DATA (0XBC807527ED3E12BD) ,DATA (0XEBA09271E88D976C) ,
					DATA (0X93445B8731587EA3) ,DATA (0XB8157268FDAE9E4C) ,DATA (0XE61ACF033D1A45DF) ,
					DATA (0X8FD0C16206306BAC) ,DATA (0XB3C4F1BA87BC8697) ,DATA (0XE0B62E2929ABA83C) ,
					DATA (0X8C71DCD9BA0B4926) ,DATA (0XAF8E5410288E1B6F) ,DATA (0XDB71E91432B1A24B) ,
					DATA (0X892731AC9FAF056F) ,DATA (0XAB70FE17C79AC6CA) ,DATA (0XD64D3D9DB981787D) ,
					DATA (0X85F0468293F0EB4E) ,DATA (0XA76C582338ED2622) ,DATA (0XD1476E2C07286FAA) ,
					DATA (0X82CCA4DB847945CA) ,DATA (0XA37FCE126597973D) ,DATA (0XCC5FC196FEFD7D0C) ,
					DATA (0XFF77B1FCBEBCDC4F) ,DATA (0X9FAACF3DF73609B1) ,DATA (0XC795830D75038C1E) ,
					DATA (0XF97AE3D0D2446F25) ,DATA (0X9BECCE62836AC577) ,DATA (0XC2E801FB244576D5) ,
					DATA (0XF3A20279ED56D48A) ,DATA (0X9845418C345644D7) ,DATA (0XBE5691EF416BD60C) ,
					DATA (0XEDEC366B11C6CB8F) ,DATA (0X94B3A202EB1C3F39) ,DATA (0XB9E08A83A5E34F08) ,
					DATA (0XE858AD248F5C22CA) ,DATA (0X91376C36D99995BE) ,DATA (0XB58547448FFFFB2E) ,
					DATA (0XE2E69915B3FFF9F9) ,DATA (0X8DD01FAD907FFC3C) ,DATA (0XB1442798F49FFB4B) ,
					DATA (0XDD95317F31C7FA1D) ,DATA (0X8A7D3EEF7F1CFC52) ,DATA (0XAD1C8EAB5EE43B67) ,
					DATA (0XD863B256369D4A41) ,DATA (0X873E4F75E2224E68) ,DATA (0XA90DE3535AAAE202) ,
					DATA (0XD3515C2831559A83) ,DATA (0X8412D9991ED58092) ,DATA (0XA5178FFF668AE0B6) ,
					DATA (0XCE5D73FF402D98E4) ,DATA (0X80FA687F881C7F8E) ,DATA (0XA139029F6A239F72) ,
					DATA (0XC987434744AC874F) ,DATA (0XFBE9141915D7A922) ,DATA (0X9D71AC8FADA6C9B5) ,
					DATA (0XC4CE17B399107C23) ,DATA (0XF6019DA07F549B2B) ,DATA (0X99C102844F94E0FB) ,
					DATA (0XC0314325637A193A) ,DATA (0XF03D93EEBC589F88) ,DATA (0X96267C7535B763B5) ,
					DATA (0XBBB01B9283253CA3) ,DATA (0XEA9C227723EE8BCB) ,DATA (0X92A1958A7675175F) ,
					DATA (0XB749FAED14125D37) ,DATA (0XE51C79A85916F485) ,DATA (0X8F31CC0937AE58D3) ,
					DATA (0XB2FE3F0B8599EF08) ,DATA (0XDFBDCECE67006AC9) ,DATA (0X8BD6A141006042BE) ,
					DATA (0XAECC49914078536D) ,DATA (0XDA7F5BF590966849) ,DATA (0X888F99797A5E012D) ,
					DATA (0XAAB37FD7D8F58179) ,DATA (0XD5605FCDCF32E1D7) ,DATA (0X855C3BE0A17FCD26) ,
					DATA (0XA6B34AD8C9DFC070) ,DATA (0XD0601D8EFC57B08C) ,DATA (0X823C12795DB6CE57) ,
					DATA (0XA2CB1717B52481ED) ,DATA (0XCB7DDCDDA26DA269) ,DATA (0XFE5D54150B090B03) ,
					DATA (0X9EFA548D26E5A6E2) ,DATA (0XC6B8E9B0709F109A) ,DATA (0XF867241C8CC6D4C1) ,
					DATA (0X9B407691D7FC44F8) ,DATA (0XC21094364DFB5637) ,DATA (0XF294B943E17A2BC4) ,
					DATA (0X979CF3CA6CEC5B5B) ,DATA (0XBD8430BD08277231) ,DATA (0XECE53CEC4A314EBE) ,
					DATA (0X940F4613AE5ED137) ,DATA (0XB913179899F68584) ,DATA (0XE757DD7EC07426E5) ,
					DATA (0X9096EA6F3848984F) ,DATA (0XB4BCA50B065ABE63) ,DATA (0XE1EBCE4DC7F16DFC) ,
					DATA (0X8D3360F09CF6E4BD) ,DATA (0XB080392CC4349DED) ,DATA (0XDCA04777F541C568) ,
					DATA (0X89E42CAAF9491B61) ,DATA (0XAC5D37D5B79B6239) ,DATA (0XD77485CB25823AC7) ,
					DATA (0X86A8D39EF77164BD) ,DATA (0XA8530886B54DBDEC) ,DATA (0XD267CAA862A12D67) ,
					DATA (0X8380DEA93DA4BC60) ,DATA (0XA46116538D0DEB78) ,DATA (0XCD795BE870516656) ,
					DATA (0X806BD9714632DFF6) ,DATA (0XA086CFCD97BF97F4) ,DATA (0XC8A883C0FDAF7DF0) ,
					DATA (0XFAD2A4B13D1B5D6C) ,DATA (0X9CC3A6EEC6311A64) ,DATA (0XC3F490AA77BD60FD) ,
					DATA (0XF4F1B4D515ACB93C) ,DATA (0X991711052D8BF3C5) ,DATA (0XBF5CD54678EEF0B7) ,
					DATA (0XEF340A98172AACE5) ,DATA (0X9580869F0E7AAC0F) ,DATA (0XBAE0A846D2195713) ,
					DATA (0XE998D258869FACD7) ,DATA (0X91FF83775423CC06) ,DATA (0XB67F6455292CBF08) ,
					DATA (0XE41F3D6A7377EECA) ,DATA (0X8E938662882AF53E) ,DATA (0XB23867FB2A35B28E) ,
					DATA (0XDEC681F9F4C31F31) ,DATA (0X8B3C113C38F9F37F) ,DATA (0XAE0B158B4738705F) ,
					DATA (0XD98DDAEE19068C76) ,DATA (0X87F8A8D4CFA417CA) ,DATA (0XA9F6D30A038D1DBC) ,
					DATA (0XD47487CC8470652B) ,DATA (0X84C8D4DFD2C63F3B) ,DATA (0XA5FB0A17C777CF0A) ,
					DATA (0XCF79CC9DB955C2CC) ,DATA (0X81AC1FE293D599C0) ,DATA (0XA21727DB38CB0030) ,
					DATA (0XCA9CF1D206FDC03C) ,DATA (0XFD442E4688BD304B) ,DATA (0X9E4A9CEC15763E2F) ,
					DATA (0XC5DD44271AD3CDBA) ,DATA (0XF7549530E188C129) ,DATA (0X9A94DD3E8CF578BA) ,
					DATA (0XC13A148E3032D6E8) ,DATA (0XF18899B1BC3F8CA2) ,DATA (0X96F5600F15A7B7E5) ,
					DATA (0XBCB2B812DB11A5DE) ,DATA (0XEBDF661791D60F56) ,DATA (0X936B9FCEBB25C996) ,
					DATA (0XB84687C269EF3BFB) ,DATA (0XE65829B3046B0AFA) ,DATA (0X8FF71A0FE2C2E6DC) ,
					DATA (0XB3F4E093DB73A093) ,DATA (0XE0F218B8D25088B8) ,DATA (0X8C974F7383725573) ,
					DATA (0XAFBD2350644EEAD0) ,DATA (0XDBAC6C247D62A584) ,DATA (0X894BC396CE5DA772) ,
					DATA (0XAB9EB47C81F5114F) ,DATA (0XD686619BA27255A3) ,DATA (0X8613FD0145877586) ,
					DATA (0XA798FC4196E952E7) ,DATA (0XD17F3B51FCA3A7A1) ,DATA (0X82EF85133DE648C5) ,
					DATA (0XA3AB66580D5FDAF6) ,DATA (0XCC963FEE10B7D1B3) ,DATA (0XFFBBCFE994E5C620) ,
					DATA (0X9FD561F1FD0F9BD4) ,DATA (0XC7CABA6E7C5382C9) ,DATA (0XF9BD690A1B68637B) ,
					DATA (0X9C1661A651213E2D) ,DATA (0XC31BFA0FE5698DB8) ,DATA (0XF3E2F893DEC3F126) ,
					DATA (0X986DDB5C6B3A76B8) ,DATA (0XBE89523386091466) ,DATA (0XEE2BA6C0678B597F) ,
					DATA (0X94DB483840B717F0) ,DATA (0XBA121A4650E4DDEC) ,DATA (0XE896A0D7E51E1566) ,
					DATA (0X915E2486EF32CD60) ,DATA (0XB5B5ADA8AAFF80B8) ,DATA (0XE3231912D5BF60E6) ,
					DATA (0X8DF5EFABC5979C90) ,DATA (0XB1736B96B6FD83B4) ,DATA (0XDDD0467C64BCE4A1) ,
					DATA (0X8AA22C0DBEF60EE4) ,DATA (0XAD4AB7112EB3929E) ,DATA (0XD89D64D57A607745) ,
					DATA (0X87625F056C7C4A8B) ,DATA (0XA93AF6C6C79B5D2E) ,DATA (0XD389B47879823479) ,
					DATA (0X843610CB4BF160CC) ,DATA (0XA54394FE1EEDB8FF) ,DATA (0XCE947A3DA6A9273E) ,
					DATA (0X811CCC668829B887) ,DATA (0XA163FF802A3426A9) ,DATA (0XC9BCFF6034C13053) ,
					DATA (0XFC2C3F3841F17C68) ,DATA (0X9D9BA7832936EDC1) ,DATA (0XC5029163F384A931) ,
					DATA (0XF64335BCF065D37D) ,DATA (0X99EA0196163FA42E) ,DATA (0XC06481FB9BCF8D3A) ,
					DATA (0XF07DA27A82C37088) ,DATA (0X964E858C91BA2655) ,DATA (0XBBE226EFB628AFEB) ,
					DATA (0XEADAB0ABA3B2DBE5) ,DATA (0X92C8AE6B464FC96F) ,DATA (0XB77ADA0617E3BBCB) ,
					DATA (0XE55990879DDCAABE) ,DATA (0X8F57FA54C2A9EAB7) ,DATA (0XB32DF8E9F3546564) ,
					DATA (0XDFF9772470297EBD) ,DATA (0X8BFBEA76C619EF36) ,DATA (0XAEFAE51477A06B04) ,
					DATA (0XDAB99E59958885C5) ,DATA (0X88B402F7FD75539B) ,DATA (0XAAE103B5FCD2A882) ,
					DATA (0XD59944A37C0752A2) ,DATA (0X857FCAE62D8493A5) ,DATA (0XA6DFBD9FB8E5B88F) ,
					DATA (0XD097AD07A71F26B2) ,DATA (0X825ECC24C8737830) ,DATA (0XA2F67F2DFA90563B) ,
					DATA (0XCBB41EF979346BCA) ,DATA (0XFEA126B7D78186BD) ,DATA (0X9F24B832E6B0F436) ,
					DATA (0XC6EDE63FA05D3144) ,DATA (0XF8A95FCF88747D94) ,DATA (0X9B69DBE1B548CE7D) ,
					DATA (0XC24452DA229B021C) ,DATA (0XF2D56790AB41C2A3) ,DATA (0X97C560BA6B0919A6) ,
					DATA (0XBDB6B8E905CB600F) ,DATA (0XED246723473E3813) ,DATA (0X9436C0760C86E30C) ,
					DATA (0XB94470938FA89BCF) ,DATA (0XE7958CB87392C2C3) ,DATA (0X90BD77F3483BB9BA) ,
					DATA (0XB4ECD5F01A4AA828) ,DATA (0XE2280B6C20DD5232) ,DATA (0X8D590723948A535F) ,
					DATA (0XB0AF48EC79ACE837) ,DATA (0XDCDB1B2798182245) ,DATA (0X8A08F0F8BF0F156B) ,
					DATA (0XAC8B2D36EED2DAC6) ,DATA (0XD7ADF884AA879177) ,DATA (0X86CCBB52EA94BAEB) ,
					DATA (0XA87FEA27A539E9A5) ,DATA (0XD29FE4B18E88640F) ,DATA (0X83A3EEEEF9153E89) ,
					DATA (0XA48CEAAAB75A8E2B) ,DATA (0XCDB02555653131B6) ,DATA (0X808E17555F3EBF12) ,
					DATA (0XA0B19D2AB70E6ED6) ,DATA (0XC8DE047564D20A8C) ,DATA (0XFB158592BE068D2F) ,
					DATA (0X9CED737BB6C4183D) ,DATA (0XC428D05AA4751E4D) ,DATA (0XF53304714D9265E0) ,
					DATA (0X993FE2C6D07B7FAC) ,DATA (0XBF8FDB78849A5F97) ,DATA (0XEF73D256A5C0F77D) ,
					DATA (0X95A8637627989AAE) ,DATA (0XBB127C53B17EC159) ,DATA (0XE9D71B689DDE71B0) ,
					DATA (0X9226712162AB070E) ,DATA (0XB6B00D69BB55C8D1) ,DATA (0XE45C10C42A2B3B06) ,
					DATA (0X8EB98A7A9A5B04E3) ,DATA (0XB267ED1940F1C61C) ,DATA (0XDF01E85F912E37A3) ,
					DATA (0X8B61313BBABCE2C6) ,DATA (0XAE397D8AA96C1B78) ,DATA (0XD9C7DCED53C72256) ,
					DATA (0X881CEA14545C7575) ,DATA (0XAA242499697392D3) ,DATA (0XD4AD2DBFC3D07788) ,
					DATA (0X84EC3C97DA624AB5) ,DATA (0XA6274BBDD0FADD62) ,DATA (0XCFB11EAD453994BA) ,
					DATA (0X81CEB32C4B43FCF5) ,DATA (0XA2425FF75E14FC32) ,DATA (0XCAD2F7F5359A3B3E) ,
					DATA (0XFD87B5F28300CA0E) ,DATA (0X9E74D1B791E07E48) ,DATA (0XC612062576589DDB) ,
					DATA (0XF79687AED3EEC551) ,DATA (0X9ABE14CD44753B53) ,DATA (0XC16D9A0095928A27) ,
					DATA (0XF1C90080BAF72CB1) ,DATA (0X971DA05074DA7BEF) ,DATA (0XBCE5086492111AEB) ,
					DATA (0XEC1E4A7DB69561A5) ,DATA (0X9392EE8E921D5D07) ,DATA (0XB877AA3236A4B449) ,
					DATA (0XE69594BEC44DE15B) ,DATA (0X901D7CF73AB0ACD9) ,DATA (0XB424DC35095CD80F) ,
					DATA (0XE12E13424BB40E13) ,DATA (0X8CBCCC096F5088CC) ,DATA (0XAFEBFF0BCB24AAFF) ,
					DATA (0XDBE6FECEBDEDD5BF) ,DATA (0X89705F4136B4A597) ,DATA (0XABCC77118461CEFD) ,
					DATA (0XD6BF94D5E57A42BC) ,DATA (0X8637BD05AF6C69B6) ,DATA (0XA7C5AC471B478423) ,
					DATA (0XD1B71758E219652C) ,DATA (0X83126E978D4FDF3B) ,DATA (0XA3D70A3D70A3D70A) ,
					DATA (0XCCCCCCCCCCCCCCCD) ,DATA (0X8000000000000000) ,DATA (0XA000000000000000) ,
					DATA (0XC800000000000000) ,DATA (0XFA00000000000000) ,DATA (0X9C40000000000000) ,
					DATA (0XC350000000000000) ,DATA (0XF424000000000000) ,DATA (0X9896800000000000) ,
					DATA (0XBEBC200000000000) ,DATA (0XEE6B280000000000) ,DATA (0X9502F90000000000) ,
					DATA (0XBA43B74000000000) ,DATA (0XE8D4A51000000000) ,DATA (0X9184E72A00000000) ,
					DATA (0XB5E620F480000000) ,DATA (0XE35FA931A0000000) ,DATA (0X8E1BC9BF04000000) ,
					DATA (0XB1A2BC2EC5000000) ,DATA (0XDE0B6B3A76400000) ,DATA (0X8AC7230489E80000) ,
					DATA (0XAD78EBC5AC620000) ,DATA (0XD8D726B7177A8000) ,DATA (0X878678326EAC9000) ,
					DATA (0XA968163F0A57B400) ,DATA (0XD3C21BCECCEDA100) ,DATA (0X84595161401484A0) ,
					DATA (0XA56FA5B99019A5C8) ,DATA (0XCECB8F27F4200F3A) ,DATA (0X813F3978F8940984) ,
					DATA (0XA18F07D736B90BE5) ,DATA (0XC9F2C9CD04674EDF) ,DATA (0XFC6F7C4045812296) ,
					DATA (0X9DC5ADA82B70B59E) ,DATA (0XC5371912364CE305) ,DATA (0XF684DF56C3E01BC7) ,
					DATA (0X9A130B963A6C115C) ,DATA (0XC097CE7BC90715B3) ,DATA (0XF0BDC21ABB48DB20) ,
					DATA (0X96769950B50D88F4) ,DATA (0XBC143FA4E250EB31) ,DATA (0XEB194F8E1AE525FD) ,
					DATA (0X92EFD1B8D0CF37BE) ,DATA (0XB7ABC627050305AE) ,DATA (0XE596B7B0C643C719) ,
					DATA (0X8F7E32CE7BEA5C70) ,DATA (0XB35DBF821AE4F38C) ,DATA (0XE0352F62A19E306F) ,
					DATA (0X8C213D9DA502DE45) ,DATA (0XAF298D050E4395D7) ,DATA (0XDAF3F04651D47B4C) ,
					DATA (0X88D8762BF324CD10) ,DATA (0XAB0E93B6EFEE0054) ,DATA (0XD5D238A4ABE98068) ,
					DATA (0X85A36366EB71F041) ,DATA (0XA70C3C40A64E6C52) ,DATA (0XD0CF4B50CFE20766) ,
					DATA (0X82818F1281ED44A0) ,DATA (0XA321F2D7226895C8) ,DATA (0XCBEA6F8CEB02BB3A) ,
					DATA (0XFEE50B7025C36A08) ,DATA (0X9F4F2726179A2245) ,DATA (0XC722F0EF9D80AAD6) ,
					DATA (0XF8EBAD2B84E0D58C) ,DATA (0X9B934C3B330C8577) ,DATA (0XC2781F49FFCFA6D5) ,
					DATA (0XF316271C7FC3908B) ,DATA (0X97EDD871CFDA3A57) ,DATA (0XBDE94E8E43D0C8EC) ,
					DATA (0XED63A231D4C4FB27) ,DATA (0X945E455F24FB1CF9) ,DATA (0XB975D6B6EE39E437) ,
					DATA (0XE7D34C64A9C85D44) ,DATA (0X90E40FBEEA1D3A4B) ,DATA (0XB51D13AEA4A488DD) ,
					DATA (0XE264589A4DCDAB15) ,DATA (0X8D7EB76070A08AED) ,DATA (0XB0DE65388CC8ADA8) ,
					DATA (0XDD15FE86AFFAD912) ,DATA (0X8A2DBF142DFCC7AB) ,DATA (0XACB92ED9397BF996) ,
					DATA (0XD7E77A8F87DAF7FC) ,DATA (0X86F0AC99B4E8DAFD) ,DATA (0XA8ACD7C0222311BD) ,
					DATA (0XD2D80DB02AABD62C) ,DATA (0X83C7088E1AAB65DB) ,DATA (0XA4B8CAB1A1563F52) ,
					DATA (0XCDE6FD5E09ABCF27) ,DATA (0X80B05E5AC60B6178) ,DATA (0XA0DC75F1778E39D6) ,
					DATA (0XC913936DD571C84C) ,DATA (0XFB5878494ACE3A5F) ,DATA (0X9D174B2DCEC0E47B) ,
					DATA (0XC45D1DF942711D9A) ,DATA (0XF5746577930D6501) ,DATA (0X9968BF6ABBE85F20) ,
					DATA (0XBFC2EF456AE276E9) ,DATA (0XEFB3AB16C59B14A3) ,DATA (0X95D04AEE3B80ECE6) ,
					DATA (0XBB445DA9CA61281F) ,DATA (0XEA1575143CF97227) ,DATA (0X924D692CA61BE758) ,
					DATA (0XB6E0C377CFA2E12E) ,DATA (0XE498F455C38B997A) ,DATA (0X8EDF98B59A373FEC) ,
					DATA (0XB2977EE300C50FE7) ,DATA (0XDF3D5E9BC0F653E1) ,DATA (0X8B865B215899F46D) ,
					DATA (0XAE67F1E9AEC07188) ,DATA (0XDA01EE641A708DEA) ,DATA (0X884134FE908658B2) ,
					DATA (0XAA51823E34A7EEDF) ,DATA (0XD4E5E2CDC1D1EA96) ,DATA (0X850FADC09923329E) ,
					DATA (0XA6539930BF6BFF46) ,DATA (0XCFE87F7CEF46FF17) ,DATA (0X81F14FAE158C5F6E) ,
					DATA (0XA26DA3999AEF774A) ,DATA (0XCB090C8001AB551C) ,DATA (0XFDCB4FA002162A63) ,
					DATA (0X9E9F11C4014DDA7E) ,DATA (0XC646D63501A1511E) ,DATA (0XF7D88BC24209A565) ,
					DATA (0X9AE757596946075F) ,DATA (0XC1A12D2FC3978937) ,DATA (0XF209787BB47D6B85) ,
					DATA (0X9745EB4D50CE6333) ,DATA (0XBD176620A501FC00) ,DATA (0XEC5D3FA8CE427B00) ,
					DATA (0X93BA47C980E98CE0) ,DATA (0XB8A8D9BBE123F018) ,DATA (0XE6D3102AD96CEC1E) ,
					DATA (0X9043EA1AC7E41393) ,DATA (0XB454E4A179DD1877) ,DATA (0XE16A1DC9D8545E95) ,
					DATA (0X8CE2529E2734BB1D) ,DATA (0XB01AE745B101E9E4) ,DATA (0XDC21A1171D42645D) ,
					DATA (0X899504AE72497EBA) ,DATA (0XABFA45DA0EDBDE69) ,DATA (0XD6F8D7509292D603) ,
					DATA (0X865B86925B9BC5C2) ,DATA (0XA7F26836F282B733) ,DATA (0XD1EF0244AF2364FF) ,
					DATA (0X8335616AED761F1F) ,DATA (0XA402B9C5A8D3A6E7) ,DATA (0XCD036837130890A1) ,
					DATA (0X802221226BE55A65) ,DATA (0XA02AA96B06DEB0FE) ,DATA (0XC83553C5C8965D3D) ,
					DATA (0XFA42A8B73ABBF48D) ,DATA (0X9C69A97284B578D8) ,DATA (0XC38413CF25E2D70E) ,
					DATA (0XF46518C2EF5B8CD1) ,DATA (0X98BF2F79D5993803) ,DATA (0XBEEEFB584AFF8604) ,
					DATA (0XEEAABA2E5DBF6785) ,DATA (0X952AB45CFA97A0B3) ,DATA (0XBA756174393D88E0) ,
					DATA (0XE912B9D1478CEB17) ,DATA (0X91ABB422CCB812EF) ,DATA (0XB616A12B7FE617AA) ,
					DATA (0XE39C49765FDF9D95) ,DATA (0X8E41ADE9FBEBC27D) ,DATA (0XB1D219647AE6B31C) ,
					DATA (0XDE469FBD99A05FE3) ,DATA (0X8AEC23D680043BEE) ,DATA (0XADA72CCC20054AEA) ,
					DATA (0XD910F7FF28069DA4) ,DATA (0X87AA9AFF79042287) ,DATA (0XA99541BF57452B28) ,
					DATA (0XD3FA922F2D1675F2) ,DATA (0X847C9B5D7C2E09B7) ,DATA (0XA59BC234DB398C25) ,
					DATA (0XCF02B2C21207EF2F) ,DATA (0X8161AFB94B44F57D) ,DATA (0XA1BA1BA79E1632DC) ,
					DATA (0XCA28A291859BBF93) ,DATA (0XFCB2CB35E702AF78) ,DATA (0X9DEFBF01B061ADAB) ,
					DATA (0XC56BAEC21C7A1916) ,DATA (0XF6C69A72A3989F5C) ,DATA (0X9A3C2087A63F6399) ,
					DATA (0XC0CB28A98FCF3C80) ,DATA (0XF0FDF2D3F3C30B9F) ,DATA (0X969EB7C47859E744) ,
					DATA (0XBC4665B596706115) ,DATA (0XEB57FF22FC0C795A) ,DATA (0X9316FF75DD87CBD8) ,
					DATA (0XB7DCBF5354E9BECE) ,DATA (0XE5D3EF282A242E82) ,DATA (0X8FA475791A569D11) ,
					DATA (0XB38D92D760EC4455) ,DATA (0XE070F78D3927556B) ,DATA (0X8C469AB843B89563) ,
					DATA (0XAF58416654A6BABB) ,DATA (0XDB2E51BFE9D0696A) ,DATA (0X88FCF317F22241E2) ,
					DATA (0XAB3C2FDDEEAAD25B) ,DATA (0XD60B3BD56A5586F2) ,DATA (0X85C7056562757457) ,
					DATA (0XA738C6BEBB12D16D) ,DATA (0XD106F86E69D785C8) ,DATA (0X82A45B450226B39D) ,
					DATA (0XA34D721642B06084) ,DATA (0XCC20CE9BD35C78A5) ,DATA (0XFF290242C83396CE) ,
					DATA (0X9F79A169BD203E41) ,DATA (0XC75809C42C684DD1) ,DATA (0XF92E0C3537826146) ,
					DATA (0X9BBCC7A142B17CCC) ,DATA (0XC2ABF989935DDBFE) ,DATA (0XF356F7EBF83552FE) ,
					DATA (0X98165AF37B2153DF) ,DATA (0XBE1BF1B059E9A8D6) ,DATA (0XEDA2EE1C7064130C) ,
					DATA (0X9485D4D1C63E8BE8) ,DATA (0XB9A74A0637CE2EE1) ,DATA (0XE8111C87C5C1BA9A) ,
					DATA (0X910AB1D4DB9914A0) ,DATA (0XB54D5E4A127F59C8) ,DATA (0XE2A0B5DC971F303A) ,
					DATA (0X8DA471A9DE737E24) ,DATA (0XB10D8E1456105DAD) ,DATA (0XDD50F1996B947519) ,
					DATA (0X8A5296FFE33CC930) ,DATA (0XACE73CBFDC0BFB7B) ,DATA (0XD8210BEFD30EFA5A) ,
					DATA (0X8714A775E3E95C78) ,DATA (0XA8D9D1535CE3B396) ,DATA (0XD31045A8341CA07C) ,
					DATA (0X83EA2B892091E44E) ,DATA (0XA4E4B66B68B65D61) ,DATA (0XCE1DE40642E3F4B9) ,
					DATA (0X80D2AE83E9CE78F4) ,DATA (0XA1075A24E4421731) ,DATA (0XC94930AE1D529CFD) ,
					DATA (0XFB9B7CD9A4A7443C) ,DATA (0X9D412E0806E88AA6) ,DATA (0XC491798A08A2AD4F) ,
					DATA (0XF5B5D7EC8ACB58A3) ,DATA (0X9991A6F3D6BF1766) ,DATA (0XBFF610B0CC6EDD3F) ,
					DATA (0XEFF394DCFF8A948F) ,DATA (0X95F83D0A1FB69CD9) ,DATA (0XBB764C4CA7A44410) ,
					DATA (0XEA53DF5FD18D5514) ,DATA (0X92746B9BE2F8552C) ,DATA (0XB7118682DBB66A77) ,
					DATA (0XE4D5E82392A40515) ,DATA (0X8F05B1163BA6832D) ,DATA (0XB2C71D5BCA9023F8) ,
					DATA (0XDF78E4B2BD342CF7) ,DATA (0X8BAB8EEFB6409C1A) ,DATA (0XAE9672ABA3D0C321) ,
					DATA (0XDA3C0F568CC4F3E9) ,DATA (0X8865899617FB1871) ,DATA (0XAA7EEBFB9DF9DE8E) ,
					DATA (0XD51EA6FA85785631) ,DATA (0X8533285C936B35DF) ,DATA (0XA67FF273B8460357) ,
					DATA (0XD01FEF10A657842C) ,DATA (0X8213F56A67F6B29C) ,DATA (0XA298F2C501F45F43) ,
					DATA (0XCB3F2F7642717713) ,DATA (0XFE0EFB53D30DD4D8) ,DATA (0X9EC95D1463E8A507) ,
					DATA (0XC67BB4597CE2CE49) ,DATA (0XF81AA16FDC1B81DB) ,DATA (0X9B10A4E5E9913129) ,
					DATA (0XC1D4CE1F63F57D73) ,DATA (0XF24A01A73CF2DCD0) ,DATA (0X976E41088617CA02) ,
					DATA (0XBD49D14AA79DBC82) ,DATA (0XEC9C459D51852BA3) ,DATA (0X93E1AB8252F33B46) ,
					DATA (0XB8DA1662E7B00A17) ,DATA (0XE7109BFBA19C0C9D) ,DATA (0X906A617D450187E2) ,
					DATA (0XB484F9DC9641E9DB) ,DATA (0XE1A63853BBD26451) ,DATA (0X8D07E33455637EB3) ,
					DATA (0XB049DC016ABC5E60) ,DATA (0XDC5C5301C56B75F7) ,DATA (0X89B9B3E11B6329BB) ,
					DATA (0XAC2820D9623BF429) ,DATA (0XD732290FBACAF134) ,DATA (0X867F59A9D4BED6C0) ,
					DATA (0XA81F301449EE8C70) ,DATA (0XD226FC195C6A2F8C) ,DATA (0X83585D8FD9C25DB8) ,
					DATA (0XA42E74F3D032F526) ,DATA (0XCD3A1230C43FB26F) ,DATA (0X80444B5E7AA7CF85) ,
					DATA (0XA0555E361951C367) ,DATA (0XC86AB5C39FA63441) ,DATA (0XFA856334878FC151) ,
					DATA (0X9C935E00D4B9D8D2) ,DATA (0XC3B8358109E84F07) ,DATA (0XF4A642E14C6262C9) ,
					DATA (0X98E7E9CCCFBD7DBE) ,DATA (0XBF21E44003ACDD2D) ,DATA (0XEEEA5D5004981478) ,
					DATA (0X95527A5202DF0CCB) ,DATA (0XBAA718E68396CFFE) ,DATA (0XE950DF20247C83FD) ,
					DATA (0X91D28B7416CDD27E) ,DATA (0XB6472E511C81471E) ,DATA (0XE3D8F9E563A198E5) ,
					DATA (0X8E679C2F5E44FF8F) ,DATA (0XB201833B35D63F73) ,DATA (0XDE81E40A034BCF50) ,
					DATA (0X8B112E86420F6192) ,DATA (0XADD57A27D29339F6) ,DATA (0XD94AD8B1C7380874) ,
					DATA (0X87CEC76F1C830549) ,DATA (0XA9C2794AE3A3C69B) ,DATA (0XD433179D9C8CB841) ,
					DATA (0X849FEEC281D7F329) ,DATA (0XA5C7EA73224DEFF3) ,DATA (0XCF39E50FEAE16BF0) ,
					DATA (0X81842F29F2CCE376) ,DATA (0XA1E53AF46F801C53) ,DATA (0XCA5E89B18B602368) ,
					DATA (0XFCF62C1DEE382C42) ,DATA (0X9E19DB92B4E31BA9) ,DATA (0XC5A05277621BE294) ,
					DATA (0XF70867153AA2DB39) ,DATA (0X9A65406D44A5C903) ,DATA (0XC0FE908895CF3B44) ,
					DATA (0XF13E34AABB430A15) ,DATA (0X96C6E0EAB509E64D) ,DATA (0XBC789925624C5FE1) ,
					DATA (0XEB96BF6EBADF77D9) ,DATA (0X933E37A534CBAAE8) ,DATA (0XB80DC58E81FE95A1) ,
					DATA (0XE61136F2227E3B0A) ,DATA (0X8FCAC257558EE4E6) ,DATA (0XB3BD72ED2AF29E20) ,
					DATA (0XE0ACCFA875AF45A8) ,DATA (0X8C6C01C9498D8B89) ,DATA (0XAF87023B9BF0EE6B) ,
					DATA (0XDB68C2CA82ED2A06) ,DATA (0X892179BE91D43A44) ,DATA (0XAB69D82E364948D4)}) ;
				ret.mECache = BoxBuffer<LENGTH ,SIZE> ({
					-1203 ,-1200 ,-1196 ,
					-1193 ,-1190 ,-1186 ,
					-1183 ,-1180 ,-1176 ,
					-1173 ,-1170 ,-1166 ,
					-1163 ,-1160 ,-1156 ,
					-1153 ,-1150 ,-1146 ,
					-1143 ,-1140 ,-1136 ,
					-1133 ,-1130 ,-1127 ,
					-1123 ,-1120 ,-1117 ,
					-1113 ,-1110 ,-1107 ,
					-1103 ,-1100 ,-1097 ,
					-1093 ,-1090 ,-1087 ,
					-1083 ,-1080 ,-1077 ,
					-1073 ,-1070 ,-1067 ,
					-1063 ,-1060 ,-1057 ,
					-1053 ,-1050 ,-1047 ,
					-1043 ,-1040 ,-1037 ,
					-1034 ,-1030 ,-1027 ,
					-1024 ,-1020 ,-1017 ,
					-1014 ,-1010 ,-1007 ,
					-1004 ,-1000 ,-997 ,
					-994 ,-990 ,-987 ,
					-984 ,-980 ,-977 ,
					-974 ,-970 ,-967 ,
					-964 ,-960 ,-957 ,
					-954 ,-950 ,-947 ,
					-944 ,-940 ,-937 ,
					-934 ,-931 ,-927 ,
					-924 ,-921 ,-917 ,
					-914 ,-911 ,-907 ,
					-904 ,-901 ,-897 ,
					-894 ,-891 ,-887 ,
					-884 ,-881 ,-877 ,
					-874 ,-871 ,-867 ,
					-864 ,-861 ,-857 ,
					-854 ,-851 ,-847 ,
					-844 ,-841 ,-838 ,
					-834 ,-831 ,-828 ,
					-824 ,-821 ,-818 ,
					-814 ,-811 ,-808 ,
					-804 ,-801 ,-798 ,
					-794 ,-791 ,-788 ,
					-784 ,-781 ,-778 ,
					-774 ,-771 ,-768 ,
					-764 ,-761 ,-758 ,
					-754 ,-751 ,-748 ,
					-744 ,-741 ,-738 ,
					-735 ,-731 ,-728 ,
					-725 ,-721 ,-718 ,
					-715 ,-711 ,-708 ,
					-705 ,-701 ,-698 ,
					-695 ,-691 ,-688 ,
					-685 ,-681 ,-678 ,
					-675 ,-671 ,-668 ,
					-665 ,-661 ,-658 ,
					-655 ,-651 ,-648 ,
					-645 ,-642 ,-638 ,
					-635 ,-632 ,-628 ,
					-625 ,-622 ,-618 ,
					-615 ,-612 ,-608 ,
					-605 ,-602 ,-598 ,
					-595 ,-592 ,-588 ,
					-585 ,-582 ,-578 ,
					-575 ,-572 ,-568 ,
					-565 ,-562 ,-558 ,
					-555 ,-552 ,-549 ,
					-545 ,-542 ,-539 ,
					-535 ,-532 ,-529 ,
					-525 ,-522 ,-519 ,
					-515 ,-512 ,-509 ,
					-505 ,-502 ,-499 ,
					-495 ,-492 ,-489 ,
					-485 ,-482 ,-479 ,
					-475 ,-472 ,-469 ,
					-465 ,-462 ,-459 ,
					-455 ,-452 ,-449 ,
					-446 ,-442 ,-439 ,
					-436 ,-432 ,-429 ,
					-426 ,-422 ,-419 ,
					-416 ,-412 ,-409 ,
					-406 ,-402 ,-399 ,
					-396 ,-392 ,-389 ,
					-386 ,-382 ,-379 ,
					-376 ,-372 ,-369 ,
					-366 ,-362 ,-359 ,
					-356 ,-353 ,-349 ,
					-346 ,-343 ,-339 ,
					-336 ,-333 ,-329 ,
					-326 ,-323 ,-319 ,
					-316 ,-313 ,-309 ,
					-306 ,-303 ,-299 ,
					-296 ,-293 ,-289 ,
					-286 ,-283 ,-279 ,
					-276 ,-273 ,-269 ,
					-266 ,-263 ,-259 ,
					-256 ,-253 ,-250 ,
					-246 ,-243 ,-240 ,
					-236 ,-233 ,-230 ,
					-226 ,-223 ,-220 ,
					-216 ,-213 ,-210 ,
					-206 ,-203 ,-200 ,
					-196 ,-193 ,-190 ,
					-186 ,-183 ,-180 ,
					-176 ,-173 ,-170 ,
					-166 ,-163 ,-160 ,
					-157 ,-153 ,-150 ,
					-147 ,-143 ,-140 ,
					-137 ,-133 ,-130 ,
					-127 ,-123 ,-120 ,
					-117 ,-113 ,-110 ,
					-107 ,-103 ,-100 ,
					-97 ,-93 ,-90 ,
					-87 ,-83 ,-80 ,
					-77 ,-73 ,-70 ,
					-67 ,-63 ,-60 ,
					-57 ,-54 ,-50 ,
					-47 ,-44 ,-40 ,
					-37 ,-34 ,-30 ,
					-27 ,-24 ,-20 ,
					-17 ,-14 ,-10 ,
					-7 ,-4 ,0 ,
					3 ,6 ,10 ,
					13 ,16 ,20 ,
					23 ,26 ,30 ,
					33 ,36 ,39 ,
					43 ,46 ,49 ,
					53 ,56 ,59 ,
					63 ,66 ,69 ,
					73 ,76 ,79 ,
					83 ,86 ,89 ,
					93 ,96 ,99 ,
					103 ,106 ,109 ,
					113 ,116 ,119 ,
					123 ,126 ,129 ,
					132 ,136 ,139 ,
					142 ,146 ,149 ,
					152 ,156 ,159 ,
					162 ,166 ,169 ,
					172 ,176 ,179 ,
					182 ,186 ,189 ,
					192 ,196 ,199 ,
					202 ,206 ,209 ,
					212 ,216 ,219 ,
					222 ,226 ,229 ,
					232 ,235 ,239 ,
					242 ,245 ,249 ,
					252 ,255 ,259 ,
					262 ,265 ,269 ,
					272 ,275 ,279 ,
					282 ,285 ,289 ,
					292 ,295 ,299 ,
					302 ,305 ,309 ,
					312 ,315 ,319 ,
					322 ,325 ,328 ,
					332 ,335 ,338 ,
					342 ,345 ,348 ,
					352 ,355 ,358 ,
					362 ,365 ,368 ,
					372 ,375 ,378 ,
					382 ,385 ,388 ,
					392 ,395 ,398 ,
					402 ,405 ,408 ,
					412 ,415 ,418 ,
					422 ,425 ,428 ,
					431 ,435 ,438 ,
					441 ,445 ,448 ,
					451 ,455 ,458 ,
					461 ,465 ,468 ,
					471 ,475 ,478 ,
					481 ,485 ,488 ,
					491 ,495 ,498 ,
					501 ,505 ,508 ,
					511 ,515 ,518 ,
					521 ,524 ,528 ,
					531 ,534 ,538 ,
					541 ,544 ,548 ,
					551 ,554 ,558 ,
					561 ,564 ,568 ,
					571 ,574 ,578 ,
					581 ,584 ,588 ,
					591 ,594 ,598 ,
					601 ,604 ,608 ,
					611 ,614 ,617 ,
					621 ,624 ,627 ,
					631 ,634 ,637 ,
					641 ,644 ,647 ,
					651 ,654 ,657 ,
					661 ,664 ,667 ,
					671 ,674 ,677 ,
					681 ,684 ,687 ,
					691 ,694 ,697 ,
					701 ,704 ,707 ,
					711 ,714 ,717 ,
					720 ,724 ,727 ,
					730 ,734 ,737 ,
					740 ,744 ,747 ,
					750 ,754 ,757 ,
					760 ,764 ,767 ,
					770 ,774 ,777 ,
					780 ,784 ,787 ,
					790 ,794 ,797 ,
					800 ,804 ,807 ,
					810 ,813 ,817 ,
					820 ,823 ,827 ,
					830 ,833 ,837 ,
					840 ,843 ,847 ,
					850 ,853 ,857 ,
					860 ,863 ,867 ,
					870 ,873 ,877 ,
					880 ,883 ,887 ,
					890 ,893 ,897 ,
					900 ,903 ,907 ,
					910 ,913 ,916 ,
					920 ,923 ,926 ,
					930 ,933 ,936 ,
					940 ,943 ,946 ,
					950 ,953 ,956 ,
					960 ,963 ,966 ,
					970 ,973 ,976 ,
					980 ,983 ,986 ,
					990 ,993 ,996 ,
					1000 ,1003 ,1006 ,
					1009 ,1013 ,1016 ,
					1019 ,1023 ,1026 ,
					1029 ,1033 ,1036 ,
					1039 ,1043 ,1046 ,
					1049 ,1053 ,1056 ,
					1059 ,1063 ,1066 ,
					1069 ,1073 ,1076}) ;
				return move (ret) ;
			}) ;
		}

		inline NOTATION operator[] (CREF<VAL64> k) const {
			NOTATION ret ;
			ret.mRadix = 2 ;
			ret.mSign = FALSE ;
			ret.mPrecision = 0 ;
			const auto r1x = OFFSET::value + INDEX (k) ;
			ret.mMantissa = VAL64 (mMCache[r1x]) ;
			ret.mExponent = VAL64 (mECache[r1x]) ;
			return move (ret) ;
		}
	} ;
} ;

template <>
trait FLOATPROC_IMPLHOLDER_HELP<ALWAYS> {
	using NOTATION = typename FLOATPROC_HELP<ALWAYS>::NOTATION ;
	using Holder = typename FLOATPROC_HELP<ALWAYS>::Holder ;

	class ImplHolder implement Holder {
	public:
		implicit ImplHolder () = default ;

		DOUBLE encode (CREF<NOTATION> fexp2) const override {
			assert (fexp2.mRadix == 2) ;
			auto rax = fexp2 ;
			if ifswitch (TRUE) {
				if (rax.mMantissa == 0)
					discard ;
				while (TRUE) {
					if ((DATA (rax.mMantissa) & ~DATA (0X001FFFFFFFFFFFFF)) == DATA (0X00))
						break ;
					rax.mMantissa = VAL64 (DATA (rax.mMantissa) >> 1) ;
					rax.mExponent++ ;
				}
				while (TRUE) {
					if ((DATA (rax.mMantissa) & ~DATA (0X000FFFFFFFFFFFFF)) != DATA (0X00))
						break ;
					rax.mMantissa = VAL64 (DATA (rax.mMantissa) << 1) ;
					rax.mExponent-- ;
				}
			}
			if ifswitch (TRUE) {
				const auto r1x = VAL64 (-1074) - rax.mExponent ;
				if (r1x <= 0)
					discard ;
				rax.mMantissa = VAL64 (DATA (rax.mMantissa) >> r1x) ;
				rax.mExponent = -1075 ;
			}
			rax.mExponent += 1075 ;
			if ifswitch (TRUE) {
				if (rax.mMantissa != 0)
					discard ;
				rax.mExponent = 0 ;
			}
			const auto r2x = invoke ([&] () {
				if ifnot (fexp2.mSign)
					return DATA (0X00) ;
				return DATA (0X8000000000000000) ;
			}) ;
			const auto r3x = (DATA (rax.mExponent) << 52) & DATA (0X7FF0000000000000) ;
			const auto r4x = DATA (rax.mMantissa) & DATA (0X000FFFFFFFFFFFFF) ;
			const auto r5x = r2x | r3x | r4x ;
			return bitwise (TYPEAS<DOUBLE>::id ,r5x) ;
		}

		NOTATION decode (CREF<DOUBLE> float_) const override {
			NOTATION ret ;
			ret.mRadix = 2 ;
			const auto r1x = bitwise (float_) ;
			const auto r2x = r1x & DATA (0X8000000000000000) ;
			const auto r3x = r1x & DATA (0X7FF0000000000000) ;
			const auto r4x = r1x & DATA (0X000FFFFFFFFFFFFF) ;
			ret.mSign = BOOL (r2x != DATA (0X00)) ;
			ret.mPrecision = 0 ;
			ret.mMantissa = VAL64 (r4x) ;
			if ifswitch (TRUE) {
				if (r3x == DATA (0X00))
					discard ;
				ret.mMantissa = VAL64 (DATA (ret.mMantissa) | DATA (0X0010000000000000)) ;
			}
			ret.mExponent = VAL64 (r3x >> 52) ;
			ret.mExponent -= 1075 - LENGTH (r3x == DATA (0X00)) ;
			if ifswitch (TRUE) {
				if (ret.mMantissa != 0)
					discard ;
				ret.mExponent = 0 ;
			}
			if ifswitch (TRUE) {
				if (ret.mMantissa == 0)
					discard ;
				while (TRUE) {
					if ((DATA (ret.mMantissa) & DATA (0X0000000000000001)) != DATA (0X00))
						break ;
					ret.mMantissa = VAL64 (DATA (ret.mMantissa) >> 1) ;
					ret.mExponent++ ;
				}
			}
			return move (ret) ;
		}

		NOTATION multiply_notation (CREF<NOTATION> a ,CREF<NOTATION> b) const {
			NOTATION ret ;
			ret.mRadix = 2 ;
			ret.mSign = FALSE ;
			ret.mPrecision = 0 ;
			const auto r1x = LENGTH (32) ;
			const auto r2x = DATA (VAL64 (DATA (0X01) << r1x) - 1) ;
			const auto r3x = DATA (0X01) << (r1x - 1) ;
			const auto r4x = VAL64 (DATA (a.mMantissa) >> r1x) ;
			const auto r5x = VAL64 (DATA (a.mMantissa) & r2x) ;
			const auto r6x = VAL64 (DATA (b.mMantissa) >> r1x) ;
			const auto r7x = VAL64 (DATA (b.mMantissa) & r2x) ;
			const auto r8x = r4x * r6x ;
			const auto r9x = VAL64 (DATA (r5x * r6x) >> r1x) ;
			const auto r10x = VAL64 (DATA (r5x * r6x) & r2x) ;
			const auto r11x = VAL64 (DATA (r4x * r7x) >> r1x) ;
			const auto r12x = VAL64 (DATA (r4x * r7x) & r2x) ;
			const auto r13x = VAL64 (DATA (r5x * r7x) >> r1x) ;
			const auto r14x = VAL64 (DATA (r13x + r10x + r12x + VAL64 (r3x)) >> r1x) ;
			ret.mMantissa = r8x + r9x + r11x + r14x ;
			ret.mExponent = a.mExponent + b.mExponent + r1x * 2 ;
			return move (ret) ;
		}

		NOTATION exp2_from_exp10 (CREF<NOTATION> fexp10) const override {
			using R1X = typename FLOATPROC_KROSHCACHE_HELP<ALWAYS>::KroshCache ;
			assert (fexp10.mRadix == 10) ;
			const auto r1x = invoke ([&] () {
				NOTATION ret ;
				ret.mRadix = 2 ;
				ret.mSign = FALSE ;
				ret.mPrecision = 0 ;
				ret.mMantissa = fexp10.mMantissa ;
				ret.mExponent = 0 ;
				while (TRUE) {
					if ((DATA (ret.mMantissa) & ~DATA (0X7FFFFFFFFFFFFFFF)) != DATA (0X00))
						break ;
					ret.mMantissa = VAL64 (DATA (ret.mMantissa) << 1) ;
					ret.mExponent-- ;
				}
				return move (ret) ;
			}) ;
			const auto r2x = R1X::instance ()[fexp10.mExponent] ;
			const auto r3x = multiply_notation (r1x ,r2x) ;
			return r3x ;
		}

		NOTATION exp10_from_exp2 (CREF<NOTATION> fexp2) const override {
			assert (fexp2.mRadix == 2) ;
			NOTATION ret ;
			ret.mRadix = 10 ;
			ret.mSign = fexp2.mSign ;
			ret.mPrecision = 0 ;
			const auto r1x = DOUBLE (MATH_LN2 / MATH_LN10) * DOUBLE (fexp2.mExponent) ;
			const auto r2x = MathProc::floor (r1x ,DOUBLE (1)) ;
			//@mark
			const auto r3x = DOUBLE (fexp2.mMantissa) * MathProc::exp (DOUBLE (MATH_LN10) * (r1x - r2x)) ;
			const auto r4x = MathProc::round (r3x ,DOUBLE (1)) ;
			ret.mMantissa = VAL64 (r4x) ;
			ret.mExponent = VAL64 (r2x) ;
			return move (ret) ;
		}
	} ;
} ;

exports auto FLOATPROC_HELP<ALWAYS>::FUNCTION_link::invoke () -> VRef<Holder> {
	using R1X = typename FLOATPROC_IMPLHOLDER_HELP<ALWAYS>::ImplHolder ;
	return VRef<R1X>::make () ;
} ;
} ;
} ;