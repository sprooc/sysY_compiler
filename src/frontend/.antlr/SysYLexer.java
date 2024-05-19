// Generated from /home/sprooc/sysY-compiler/src/frontend/SysYLexer.g4 by ANTLR 4.13.1
import org.antlr.v4.runtime.Lexer;
import org.antlr.v4.runtime.CharStream;
import org.antlr.v4.runtime.Token;
import org.antlr.v4.runtime.TokenStream;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.atn.*;
import org.antlr.v4.runtime.dfa.DFA;
import org.antlr.v4.runtime.misc.*;

@SuppressWarnings({"all", "warnings", "unchecked", "unused", "cast", "CheckReturnValue", "this-escape"})
public class SysYLexer extends Lexer {
	static { RuntimeMetaData.checkVersion("4.13.1", RuntimeMetaData.VERSION); }

	protected static final DFA[] _decisionToDFA;
	protected static final PredictionContextCache _sharedContextCache =
		new PredictionContextCache();
	public static final int
		CONST=1, INT=2, VOID=3, FLOAT=4, IF=5, ELSE=6, WHILE=7, BREAK=8, CONTINUE=9, 
		RETURN=10, PLUS=11, MINUS=12, MUL=13, DIV=14, MOD=15, ASSIGN=16, EQ=17, 
		NEQ=18, LT=19, GT=20, LE=21, GE=22, NOT=23, AND=24, OR=25, L_PAREN=26, 
		R_PAREN=27, L_BRACE=28, R_BRACE=29, L_BRACKT=30, R_BRACKT=31, COMMA=32, 
		SEMICOLON=33, IDENT=34, INTEGER_CONST=35, FLOAT_CONST=36, WS=37, LINE_COMMENT=38, 
		MULTILINE_COMMENT=39;
	public static String[] channelNames = {
		"DEFAULT_TOKEN_CHANNEL", "HIDDEN"
	};

	public static String[] modeNames = {
		"DEFAULT_MODE"
	};

	private static String[] makeRuleNames() {
		return new String[] {
			"CONST", "INT", "VOID", "FLOAT", "IF", "ELSE", "WHILE", "BREAK", "CONTINUE", 
			"RETURN", "PLUS", "MINUS", "MUL", "DIV", "MOD", "ASSIGN", "EQ", "NEQ", 
			"LT", "GT", "LE", "GE", "NOT", "AND", "OR", "L_PAREN", "R_PAREN", "L_BRACE", 
			"R_BRACE", "L_BRACKT", "R_BRACKT", "COMMA", "SEMICOLON", "IDENT", "INTEGER_CONST", 
			"FLOAT_CONST", "WS", "LINE_COMMENT", "MULTILINE_COMMENT"
		};
	}
	public static final String[] ruleNames = makeRuleNames();

	private static String[] makeLiteralNames() {
		return new String[] {
			null, "'const'", "'int'", "'void'", "'float'", "'if'", "'else'", "'while'", 
			"'break'", "'continue'", "'return'", "'+'", "'-'", "'*'", "'/'", "'%'", 
			"'='", "'=='", "'!='", "'<'", "'>'", "'<='", "'>='", "'!'", "'&&'", "'||'", 
			"'('", "')'", "'{'", "'}'", "'['", "']'", "','", "';'"
		};
	}
	private static final String[] _LITERAL_NAMES = makeLiteralNames();
	private static String[] makeSymbolicNames() {
		return new String[] {
			null, "CONST", "INT", "VOID", "FLOAT", "IF", "ELSE", "WHILE", "BREAK", 
			"CONTINUE", "RETURN", "PLUS", "MINUS", "MUL", "DIV", "MOD", "ASSIGN", 
			"EQ", "NEQ", "LT", "GT", "LE", "GE", "NOT", "AND", "OR", "L_PAREN", "R_PAREN", 
			"L_BRACE", "R_BRACE", "L_BRACKT", "R_BRACKT", "COMMA", "SEMICOLON", "IDENT", 
			"INTEGER_CONST", "FLOAT_CONST", "WS", "LINE_COMMENT", "MULTILINE_COMMENT"
		};
	}
	private static final String[] _SYMBOLIC_NAMES = makeSymbolicNames();
	public static final Vocabulary VOCABULARY = new VocabularyImpl(_LITERAL_NAMES, _SYMBOLIC_NAMES);

	/**
	 * @deprecated Use {@link #VOCABULARY} instead.
	 */
	@Deprecated
	public static final String[] tokenNames;
	static {
		tokenNames = new String[_SYMBOLIC_NAMES.length];
		for (int i = 0; i < tokenNames.length; i++) {
			tokenNames[i] = VOCABULARY.getLiteralName(i);
			if (tokenNames[i] == null) {
				tokenNames[i] = VOCABULARY.getSymbolicName(i);
			}

			if (tokenNames[i] == null) {
				tokenNames[i] = "<INVALID>";
			}
		}
	}

	@Override
	@Deprecated
	public String[] getTokenNames() {
		return tokenNames;
	}

	@Override

	public Vocabulary getVocabulary() {
		return VOCABULARY;
	}


	public SysYLexer(CharStream input) {
		super(input);
		_interp = new LexerATNSimulator(this,_ATN,_decisionToDFA,_sharedContextCache);
	}

	@Override
	public String getGrammarFileName() { return "SysYLexer.g4"; }

	@Override
	public String[] getRuleNames() { return ruleNames; }

	@Override
	public String getSerializedATN() { return _serializedATN; }

	@Override
	public String[] getChannelNames() { return channelNames; }

	@Override
	public String[] getModeNames() { return modeNames; }

	@Override
	public ATN getATN() { return _ATN; }

	public static final String _serializedATN =
		"\u0004\u0000\'\u016f\u0006\uffff\uffff\u0002\u0000\u0007\u0000\u0002\u0001"+
		"\u0007\u0001\u0002\u0002\u0007\u0002\u0002\u0003\u0007\u0003\u0002\u0004"+
		"\u0007\u0004\u0002\u0005\u0007\u0005\u0002\u0006\u0007\u0006\u0002\u0007"+
		"\u0007\u0007\u0002\b\u0007\b\u0002\t\u0007\t\u0002\n\u0007\n\u0002\u000b"+
		"\u0007\u000b\u0002\f\u0007\f\u0002\r\u0007\r\u0002\u000e\u0007\u000e\u0002"+
		"\u000f\u0007\u000f\u0002\u0010\u0007\u0010\u0002\u0011\u0007\u0011\u0002"+
		"\u0012\u0007\u0012\u0002\u0013\u0007\u0013\u0002\u0014\u0007\u0014\u0002"+
		"\u0015\u0007\u0015\u0002\u0016\u0007\u0016\u0002\u0017\u0007\u0017\u0002"+
		"\u0018\u0007\u0018\u0002\u0019\u0007\u0019\u0002\u001a\u0007\u001a\u0002"+
		"\u001b\u0007\u001b\u0002\u001c\u0007\u001c\u0002\u001d\u0007\u001d\u0002"+
		"\u001e\u0007\u001e\u0002\u001f\u0007\u001f\u0002 \u0007 \u0002!\u0007"+
		"!\u0002\"\u0007\"\u0002#\u0007#\u0002$\u0007$\u0002%\u0007%\u0002&\u0007"+
		"&\u0001\u0000\u0001\u0000\u0001\u0000\u0001\u0000\u0001\u0000\u0001\u0000"+
		"\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0001\u0002\u0001\u0002"+
		"\u0001\u0002\u0001\u0002\u0001\u0002\u0001\u0003\u0001\u0003\u0001\u0003"+
		"\u0001\u0003\u0001\u0003\u0001\u0003\u0001\u0004\u0001\u0004\u0001\u0004"+
		"\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0005\u0001\u0006"+
		"\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0006\u0001\u0007"+
		"\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001\u0007\u0001\b\u0001"+
		"\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\b\u0001\t\u0001"+
		"\t\u0001\t\u0001\t\u0001\t\u0001\t\u0001\t\u0001\n\u0001\n\u0001\u000b"+
		"\u0001\u000b\u0001\f\u0001\f\u0001\r\u0001\r\u0001\u000e\u0001\u000e\u0001"+
		"\u000f\u0001\u000f\u0001\u0010\u0001\u0010\u0001\u0010\u0001\u0011\u0001"+
		"\u0011\u0001\u0011\u0001\u0012\u0001\u0012\u0001\u0013\u0001\u0013\u0001"+
		"\u0014\u0001\u0014\u0001\u0014\u0001\u0015\u0001\u0015\u0001\u0015\u0001"+
		"\u0016\u0001\u0016\u0001\u0017\u0001\u0017\u0001\u0017\u0001\u0018\u0001"+
		"\u0018\u0001\u0018\u0001\u0019\u0001\u0019\u0001\u001a\u0001\u001a\u0001"+
		"\u001b\u0001\u001b\u0001\u001c\u0001\u001c\u0001\u001d\u0001\u001d\u0001"+
		"\u001e\u0001\u001e\u0001\u001f\u0001\u001f\u0001 \u0001 \u0001!\u0001"+
		"!\u0005!\u00bf\b!\n!\f!\u00c2\t!\u0001\"\u0001\"\u0005\"\u00c6\b\"\n\""+
		"\f\"\u00c9\t\"\u0001\"\u0001\"\u0001\"\u0004\"\u00ce\b\"\u000b\"\f\"\u00cf"+
		"\u0001\"\u0001\"\u0001\"\u0004\"\u00d5\b\"\u000b\"\f\"\u00d6\u0003\"\u00d9"+
		"\b\"\u0001#\u0004#\u00dc\b#\u000b#\f#\u00dd\u0001#\u0001#\u0005#\u00e2"+
		"\b#\n#\f#\u00e5\t#\u0001#\u0001#\u0004#\u00e9\b#\u000b#\f#\u00ea\u0001"+
		"#\u0001#\u0003#\u00ef\b#\u0001#\u0004#\u00f2\b#\u000b#\f#\u00f3\u0003"+
		"#\u00f6\b#\u0001#\u0004#\u00f9\b#\u000b#\f#\u00fa\u0001#\u0001#\u0005"+
		"#\u00ff\b#\n#\f#\u0102\t#\u0003#\u0104\b#\u0001#\u0001#\u0003#\u0108\b"+
		"#\u0001#\u0004#\u010b\b#\u000b#\f#\u010c\u0001#\u0001#\u0001#\u0001#\u0004"+
		"#\u0113\b#\u000b#\f#\u0114\u0001#\u0001#\u0005#\u0119\b#\n#\f#\u011c\t"+
		"#\u0001#\u0001#\u0001#\u0001#\u0001#\u0004#\u0123\b#\u000b#\f#\u0124\u0001"+
		"#\u0001#\u0003#\u0129\b#\u0001#\u0004#\u012c\b#\u000b#\f#\u012d\u0003"+
		"#\u0130\b#\u0001#\u0001#\u0001#\u0001#\u0004#\u0136\b#\u000b#\f#\u0137"+
		"\u0001#\u0001#\u0005#\u013c\b#\n#\f#\u013f\t#\u0003#\u0141\b#\u0001#\u0001"+
		"#\u0003#\u0145\b#\u0001#\u0004#\u0148\b#\u000b#\f#\u0149\u0003#\u014c"+
		"\b#\u0001$\u0004$\u014f\b$\u000b$\f$\u0150\u0001$\u0001$\u0001%\u0001"+
		"%\u0001%\u0001%\u0005%\u0159\b%\n%\f%\u015c\t%\u0001%\u0001%\u0001%\u0001"+
		"%\u0001&\u0001&\u0001&\u0001&\u0005&\u0166\b&\n&\f&\u0169\t&\u0001&\u0001"+
		"&\u0001&\u0001&\u0001&\u0002\u015a\u0167\u0000\'\u0001\u0001\u0003\u0002"+
		"\u0005\u0003\u0007\u0004\t\u0005\u000b\u0006\r\u0007\u000f\b\u0011\t\u0013"+
		"\n\u0015\u000b\u0017\f\u0019\r\u001b\u000e\u001d\u000f\u001f\u0010!\u0011"+
		"#\u0012%\u0013\'\u0014)\u0015+\u0016-\u0017/\u00181\u00193\u001a5\u001b"+
		"7\u001c9\u001d;\u001e=\u001f? A!C\"E#G$I%K&M\'\u0001\u0000\u000b\u0003"+
		"\u0000AZ__az\u0004\u000009AZ__az\u0001\u000019\u0001\u000009\u0001\u0000"+
		"07\u0002\u0000XXxx\u0003\u000009AFaf\u0002\u0000EEee\u0002\u0000++--\u0002"+
		"\u0000PPpp\u0003\u0000\t\n\r\r  \u0193\u0000\u0001\u0001\u0000\u0000\u0000"+
		"\u0000\u0003\u0001\u0000\u0000\u0000\u0000\u0005\u0001\u0000\u0000\u0000"+
		"\u0000\u0007\u0001\u0000\u0000\u0000\u0000\t\u0001\u0000\u0000\u0000\u0000"+
		"\u000b\u0001\u0000\u0000\u0000\u0000\r\u0001\u0000\u0000\u0000\u0000\u000f"+
		"\u0001\u0000\u0000\u0000\u0000\u0011\u0001\u0000\u0000\u0000\u0000\u0013"+
		"\u0001\u0000\u0000\u0000\u0000\u0015\u0001\u0000\u0000\u0000\u0000\u0017"+
		"\u0001\u0000\u0000\u0000\u0000\u0019\u0001\u0000\u0000\u0000\u0000\u001b"+
		"\u0001\u0000\u0000\u0000\u0000\u001d\u0001\u0000\u0000\u0000\u0000\u001f"+
		"\u0001\u0000\u0000\u0000\u0000!\u0001\u0000\u0000\u0000\u0000#\u0001\u0000"+
		"\u0000\u0000\u0000%\u0001\u0000\u0000\u0000\u0000\'\u0001\u0000\u0000"+
		"\u0000\u0000)\u0001\u0000\u0000\u0000\u0000+\u0001\u0000\u0000\u0000\u0000"+
		"-\u0001\u0000\u0000\u0000\u0000/\u0001\u0000\u0000\u0000\u00001\u0001"+
		"\u0000\u0000\u0000\u00003\u0001\u0000\u0000\u0000\u00005\u0001\u0000\u0000"+
		"\u0000\u00007\u0001\u0000\u0000\u0000\u00009\u0001\u0000\u0000\u0000\u0000"+
		";\u0001\u0000\u0000\u0000\u0000=\u0001\u0000\u0000\u0000\u0000?\u0001"+
		"\u0000\u0000\u0000\u0000A\u0001\u0000\u0000\u0000\u0000C\u0001\u0000\u0000"+
		"\u0000\u0000E\u0001\u0000\u0000\u0000\u0000G\u0001\u0000\u0000\u0000\u0000"+
		"I\u0001\u0000\u0000\u0000\u0000K\u0001\u0000\u0000\u0000\u0000M\u0001"+
		"\u0000\u0000\u0000\u0001O\u0001\u0000\u0000\u0000\u0003U\u0001\u0000\u0000"+
		"\u0000\u0005Y\u0001\u0000\u0000\u0000\u0007^\u0001\u0000\u0000\u0000\t"+
		"d\u0001\u0000\u0000\u0000\u000bg\u0001\u0000\u0000\u0000\rl\u0001\u0000"+
		"\u0000\u0000\u000fr\u0001\u0000\u0000\u0000\u0011x\u0001\u0000\u0000\u0000"+
		"\u0013\u0081\u0001\u0000\u0000\u0000\u0015\u0088\u0001\u0000\u0000\u0000"+
		"\u0017\u008a\u0001\u0000\u0000\u0000\u0019\u008c\u0001\u0000\u0000\u0000"+
		"\u001b\u008e\u0001\u0000\u0000\u0000\u001d\u0090\u0001\u0000\u0000\u0000"+
		"\u001f\u0092\u0001\u0000\u0000\u0000!\u0094\u0001\u0000\u0000\u0000#\u0097"+
		"\u0001\u0000\u0000\u0000%\u009a\u0001\u0000\u0000\u0000\'\u009c\u0001"+
		"\u0000\u0000\u0000)\u009e\u0001\u0000\u0000\u0000+\u00a1\u0001\u0000\u0000"+
		"\u0000-\u00a4\u0001\u0000\u0000\u0000/\u00a6\u0001\u0000\u0000\u00001"+
		"\u00a9\u0001\u0000\u0000\u00003\u00ac\u0001\u0000\u0000\u00005\u00ae\u0001"+
		"\u0000\u0000\u00007\u00b0\u0001\u0000\u0000\u00009\u00b2\u0001\u0000\u0000"+
		"\u0000;\u00b4\u0001\u0000\u0000\u0000=\u00b6\u0001\u0000\u0000\u0000?"+
		"\u00b8\u0001\u0000\u0000\u0000A\u00ba\u0001\u0000\u0000\u0000C\u00bc\u0001"+
		"\u0000\u0000\u0000E\u00d8\u0001\u0000\u0000\u0000G\u014b\u0001\u0000\u0000"+
		"\u0000I\u014e\u0001\u0000\u0000\u0000K\u0154\u0001\u0000\u0000\u0000M"+
		"\u0161\u0001\u0000\u0000\u0000OP\u0005c\u0000\u0000PQ\u0005o\u0000\u0000"+
		"QR\u0005n\u0000\u0000RS\u0005s\u0000\u0000ST\u0005t\u0000\u0000T\u0002"+
		"\u0001\u0000\u0000\u0000UV\u0005i\u0000\u0000VW\u0005n\u0000\u0000WX\u0005"+
		"t\u0000\u0000X\u0004\u0001\u0000\u0000\u0000YZ\u0005v\u0000\u0000Z[\u0005"+
		"o\u0000\u0000[\\\u0005i\u0000\u0000\\]\u0005d\u0000\u0000]\u0006\u0001"+
		"\u0000\u0000\u0000^_\u0005f\u0000\u0000_`\u0005l\u0000\u0000`a\u0005o"+
		"\u0000\u0000ab\u0005a\u0000\u0000bc\u0005t\u0000\u0000c\b\u0001\u0000"+
		"\u0000\u0000de\u0005i\u0000\u0000ef\u0005f\u0000\u0000f\n\u0001\u0000"+
		"\u0000\u0000gh\u0005e\u0000\u0000hi\u0005l\u0000\u0000ij\u0005s\u0000"+
		"\u0000jk\u0005e\u0000\u0000k\f\u0001\u0000\u0000\u0000lm\u0005w\u0000"+
		"\u0000mn\u0005h\u0000\u0000no\u0005i\u0000\u0000op\u0005l\u0000\u0000"+
		"pq\u0005e\u0000\u0000q\u000e\u0001\u0000\u0000\u0000rs\u0005b\u0000\u0000"+
		"st\u0005r\u0000\u0000tu\u0005e\u0000\u0000uv\u0005a\u0000\u0000vw\u0005"+
		"k\u0000\u0000w\u0010\u0001\u0000\u0000\u0000xy\u0005c\u0000\u0000yz\u0005"+
		"o\u0000\u0000z{\u0005n\u0000\u0000{|\u0005t\u0000\u0000|}\u0005i\u0000"+
		"\u0000}~\u0005n\u0000\u0000~\u007f\u0005u\u0000\u0000\u007f\u0080\u0005"+
		"e\u0000\u0000\u0080\u0012\u0001\u0000\u0000\u0000\u0081\u0082\u0005r\u0000"+
		"\u0000\u0082\u0083\u0005e\u0000\u0000\u0083\u0084\u0005t\u0000\u0000\u0084"+
		"\u0085\u0005u\u0000\u0000\u0085\u0086\u0005r\u0000\u0000\u0086\u0087\u0005"+
		"n\u0000\u0000\u0087\u0014\u0001\u0000\u0000\u0000\u0088\u0089\u0005+\u0000"+
		"\u0000\u0089\u0016\u0001\u0000\u0000\u0000\u008a\u008b\u0005-\u0000\u0000"+
		"\u008b\u0018\u0001\u0000\u0000\u0000\u008c\u008d\u0005*\u0000\u0000\u008d"+
		"\u001a\u0001\u0000\u0000\u0000\u008e\u008f\u0005/\u0000\u0000\u008f\u001c"+
		"\u0001\u0000\u0000\u0000\u0090\u0091\u0005%\u0000\u0000\u0091\u001e\u0001"+
		"\u0000\u0000\u0000\u0092\u0093\u0005=\u0000\u0000\u0093 \u0001\u0000\u0000"+
		"\u0000\u0094\u0095\u0005=\u0000\u0000\u0095\u0096\u0005=\u0000\u0000\u0096"+
		"\"\u0001\u0000\u0000\u0000\u0097\u0098\u0005!\u0000\u0000\u0098\u0099"+
		"\u0005=\u0000\u0000\u0099$\u0001\u0000\u0000\u0000\u009a\u009b\u0005<"+
		"\u0000\u0000\u009b&\u0001\u0000\u0000\u0000\u009c\u009d\u0005>\u0000\u0000"+
		"\u009d(\u0001\u0000\u0000\u0000\u009e\u009f\u0005<\u0000\u0000\u009f\u00a0"+
		"\u0005=\u0000\u0000\u00a0*\u0001\u0000\u0000\u0000\u00a1\u00a2\u0005>"+
		"\u0000\u0000\u00a2\u00a3\u0005=\u0000\u0000\u00a3,\u0001\u0000\u0000\u0000"+
		"\u00a4\u00a5\u0005!\u0000\u0000\u00a5.\u0001\u0000\u0000\u0000\u00a6\u00a7"+
		"\u0005&\u0000\u0000\u00a7\u00a8\u0005&\u0000\u0000\u00a80\u0001\u0000"+
		"\u0000\u0000\u00a9\u00aa\u0005|\u0000\u0000\u00aa\u00ab\u0005|\u0000\u0000"+
		"\u00ab2\u0001\u0000\u0000\u0000\u00ac\u00ad\u0005(\u0000\u0000\u00ad4"+
		"\u0001\u0000\u0000\u0000\u00ae\u00af\u0005)\u0000\u0000\u00af6\u0001\u0000"+
		"\u0000\u0000\u00b0\u00b1\u0005{\u0000\u0000\u00b18\u0001\u0000\u0000\u0000"+
		"\u00b2\u00b3\u0005}\u0000\u0000\u00b3:\u0001\u0000\u0000\u0000\u00b4\u00b5"+
		"\u0005[\u0000\u0000\u00b5<\u0001\u0000\u0000\u0000\u00b6\u00b7\u0005]"+
		"\u0000\u0000\u00b7>\u0001\u0000\u0000\u0000\u00b8\u00b9\u0005,\u0000\u0000"+
		"\u00b9@\u0001\u0000\u0000\u0000\u00ba\u00bb\u0005;\u0000\u0000\u00bbB"+
		"\u0001\u0000\u0000\u0000\u00bc\u00c0\u0007\u0000\u0000\u0000\u00bd\u00bf"+
		"\u0007\u0001\u0000\u0000\u00be\u00bd\u0001\u0000\u0000\u0000\u00bf\u00c2"+
		"\u0001\u0000\u0000\u0000\u00c0\u00be\u0001\u0000\u0000\u0000\u00c0\u00c1"+
		"\u0001\u0000\u0000\u0000\u00c1D\u0001\u0000\u0000\u0000\u00c2\u00c0\u0001"+
		"\u0000\u0000\u0000\u00c3\u00c7\u0007\u0002\u0000\u0000\u00c4\u00c6\u0007"+
		"\u0003\u0000\u0000\u00c5\u00c4\u0001\u0000\u0000\u0000\u00c6\u00c9\u0001"+
		"\u0000\u0000\u0000\u00c7\u00c5\u0001\u0000\u0000\u0000\u00c7\u00c8\u0001"+
		"\u0000\u0000\u0000\u00c8\u00d9\u0001\u0000\u0000\u0000\u00c9\u00c7\u0001"+
		"\u0000\u0000\u0000\u00ca\u00d9\u00050\u0000\u0000\u00cb\u00cd\u00050\u0000"+
		"\u0000\u00cc\u00ce\u0007\u0004\u0000\u0000\u00cd\u00cc\u0001\u0000\u0000"+
		"\u0000\u00ce\u00cf\u0001\u0000\u0000\u0000\u00cf\u00cd\u0001\u0000\u0000"+
		"\u0000\u00cf\u00d0\u0001\u0000\u0000\u0000\u00d0\u00d9\u0001\u0000\u0000"+
		"\u0000\u00d1\u00d2\u00050\u0000\u0000\u00d2\u00d4\u0007\u0005\u0000\u0000"+
		"\u00d3\u00d5\u0007\u0006\u0000\u0000\u00d4\u00d3\u0001\u0000\u0000\u0000"+
		"\u00d5\u00d6\u0001\u0000\u0000\u0000\u00d6\u00d4\u0001\u0000\u0000\u0000"+
		"\u00d6\u00d7\u0001\u0000\u0000\u0000\u00d7\u00d9\u0001\u0000\u0000\u0000"+
		"\u00d8\u00c3\u0001\u0000\u0000\u0000\u00d8\u00ca\u0001\u0000\u0000\u0000"+
		"\u00d8\u00cb\u0001\u0000\u0000\u0000\u00d8\u00d1\u0001\u0000\u0000\u0000"+
		"\u00d9F\u0001\u0000\u0000\u0000\u00da\u00dc\u0007\u0003\u0000\u0000\u00db"+
		"\u00da\u0001\u0000\u0000\u0000\u00dc\u00dd\u0001\u0000\u0000\u0000\u00dd"+
		"\u00db\u0001\u0000\u0000\u0000\u00dd\u00de\u0001\u0000\u0000\u0000\u00de"+
		"\u00df\u0001\u0000\u0000\u0000\u00df\u00e3\u0005.\u0000\u0000\u00e0\u00e2"+
		"\u0007\u0003\u0000\u0000\u00e1\u00e0\u0001\u0000\u0000\u0000\u00e2\u00e5"+
		"\u0001\u0000\u0000\u0000\u00e3\u00e1\u0001\u0000\u0000\u0000\u00e3\u00e4"+
		"\u0001\u0000\u0000\u0000\u00e4\u014c\u0001\u0000\u0000\u0000\u00e5\u00e3"+
		"\u0001\u0000\u0000\u0000\u00e6\u00e8\u0005.\u0000\u0000\u00e7\u00e9\u0007"+
		"\u0003\u0000\u0000\u00e8\u00e7\u0001\u0000\u0000\u0000\u00e9\u00ea\u0001"+
		"\u0000\u0000\u0000\u00ea\u00e8\u0001\u0000\u0000\u0000\u00ea\u00eb\u0001"+
		"\u0000\u0000\u0000\u00eb\u00f5\u0001\u0000\u0000\u0000\u00ec\u00ee\u0007"+
		"\u0007\u0000\u0000\u00ed\u00ef\u0007\b\u0000\u0000\u00ee\u00ed\u0001\u0000"+
		"\u0000\u0000\u00ee\u00ef\u0001\u0000\u0000\u0000\u00ef\u00f1\u0001\u0000"+
		"\u0000\u0000\u00f0\u00f2\u0007\u0003\u0000\u0000\u00f1\u00f0\u0001\u0000"+
		"\u0000\u0000\u00f2\u00f3\u0001\u0000\u0000\u0000\u00f3\u00f1\u0001\u0000"+
		"\u0000\u0000\u00f3\u00f4\u0001\u0000\u0000\u0000\u00f4\u00f6\u0001\u0000"+
		"\u0000\u0000\u00f5\u00ec\u0001\u0000\u0000\u0000\u00f5\u00f6\u0001\u0000"+
		"\u0000\u0000\u00f6\u014c\u0001\u0000\u0000\u0000\u00f7\u00f9\u0007\u0003"+
		"\u0000\u0000\u00f8\u00f7\u0001\u0000\u0000\u0000\u00f9\u00fa\u0001\u0000"+
		"\u0000\u0000\u00fa\u00f8\u0001\u0000\u0000\u0000\u00fa\u00fb\u0001\u0000"+
		"\u0000\u0000\u00fb\u0103\u0001\u0000\u0000\u0000\u00fc\u0100\u0005.\u0000"+
		"\u0000\u00fd\u00ff\u0007\u0003\u0000\u0000\u00fe\u00fd\u0001\u0000\u0000"+
		"\u0000\u00ff\u0102\u0001\u0000\u0000\u0000\u0100\u00fe\u0001\u0000\u0000"+
		"\u0000\u0100\u0101\u0001\u0000\u0000\u0000\u0101\u0104\u0001\u0000\u0000"+
		"\u0000\u0102\u0100\u0001\u0000\u0000\u0000\u0103\u00fc\u0001\u0000\u0000"+
		"\u0000\u0103\u0104\u0001\u0000\u0000\u0000\u0104\u0105\u0001\u0000\u0000"+
		"\u0000\u0105\u0107\u0007\u0007\u0000\u0000\u0106\u0108\u0007\b\u0000\u0000"+
		"\u0107\u0106\u0001\u0000\u0000\u0000\u0107\u0108\u0001\u0000\u0000\u0000"+
		"\u0108\u010a\u0001\u0000\u0000\u0000\u0109\u010b\u0007\u0003\u0000\u0000"+
		"\u010a\u0109\u0001\u0000\u0000\u0000\u010b\u010c\u0001\u0000\u0000\u0000"+
		"\u010c\u010a\u0001\u0000\u0000\u0000\u010c\u010d\u0001\u0000\u0000\u0000"+
		"\u010d\u014c\u0001\u0000\u0000\u0000\u010e\u010f\u00050\u0000\u0000\u010f"+
		"\u0110\u0005x\u0000\u0000\u0110\u0112\u0001\u0000\u0000\u0000\u0111\u0113"+
		"\u0007\u0006\u0000\u0000\u0112\u0111\u0001\u0000\u0000\u0000\u0113\u0114"+
		"\u0001\u0000\u0000\u0000\u0114\u0112\u0001\u0000\u0000\u0000\u0114\u0115"+
		"\u0001\u0000\u0000\u0000\u0115\u0116\u0001\u0000\u0000\u0000\u0116\u011a"+
		"\u0005.\u0000\u0000\u0117\u0119\u0007\u0006\u0000\u0000\u0118\u0117\u0001"+
		"\u0000\u0000\u0000\u0119\u011c\u0001\u0000\u0000\u0000\u011a\u0118\u0001"+
		"\u0000\u0000\u0000\u011a\u011b\u0001\u0000\u0000\u0000\u011b\u014c\u0001"+
		"\u0000\u0000\u0000\u011c\u011a\u0001\u0000\u0000\u0000\u011d\u011e\u0005"+
		"0\u0000\u0000\u011e\u011f\u0005x\u0000\u0000\u011f\u0120\u0001\u0000\u0000"+
		"\u0000\u0120\u0122\u0005.\u0000\u0000\u0121\u0123\u0007\u0006\u0000\u0000"+
		"\u0122\u0121\u0001\u0000\u0000\u0000\u0123\u0124\u0001\u0000\u0000\u0000"+
		"\u0124\u0122\u0001\u0000\u0000\u0000\u0124\u0125\u0001\u0000\u0000\u0000"+
		"\u0125\u012f\u0001\u0000\u0000\u0000\u0126\u0128\u0007\t\u0000\u0000\u0127"+
		"\u0129\u0007\b\u0000\u0000\u0128\u0127\u0001\u0000\u0000\u0000\u0128\u0129"+
		"\u0001\u0000\u0000\u0000\u0129\u012b\u0001\u0000\u0000\u0000\u012a\u012c"+
		"\u0007\u0006\u0000\u0000\u012b\u012a\u0001\u0000\u0000\u0000\u012c\u012d"+
		"\u0001\u0000\u0000\u0000\u012d\u012b\u0001\u0000\u0000\u0000\u012d\u012e"+
		"\u0001\u0000\u0000\u0000\u012e\u0130\u0001\u0000\u0000\u0000\u012f\u0126"+
		"\u0001\u0000\u0000\u0000\u012f\u0130\u0001\u0000\u0000\u0000\u0130\u014c"+
		"\u0001\u0000\u0000\u0000\u0131\u0132\u00050\u0000\u0000\u0132\u0133\u0005"+
		"x\u0000\u0000\u0133\u0135\u0001\u0000\u0000\u0000\u0134\u0136\u0007\u0006"+
		"\u0000\u0000\u0135\u0134\u0001\u0000\u0000\u0000\u0136\u0137\u0001\u0000"+
		"\u0000\u0000\u0137\u0135\u0001\u0000\u0000\u0000\u0137\u0138\u0001\u0000"+
		"\u0000\u0000\u0138\u0140\u0001\u0000\u0000\u0000\u0139\u013d\u0005.\u0000"+
		"\u0000\u013a\u013c\u0007\u0006\u0000\u0000\u013b\u013a\u0001\u0000\u0000"+
		"\u0000\u013c\u013f\u0001\u0000\u0000\u0000\u013d\u013b\u0001\u0000\u0000"+
		"\u0000\u013d\u013e\u0001\u0000\u0000\u0000\u013e\u0141\u0001\u0000\u0000"+
		"\u0000\u013f\u013d\u0001\u0000\u0000\u0000\u0140\u0139\u0001\u0000\u0000"+
		"\u0000\u0140\u0141\u0001\u0000\u0000\u0000\u0141\u0142\u0001\u0000\u0000"+
		"\u0000\u0142\u0144\u0007\t\u0000\u0000\u0143\u0145\u0007\b\u0000\u0000"+
		"\u0144\u0143\u0001\u0000\u0000\u0000\u0144\u0145\u0001\u0000\u0000\u0000"+
		"\u0145\u0147\u0001\u0000\u0000\u0000\u0146\u0148\u0007\u0006\u0000\u0000"+
		"\u0147\u0146\u0001\u0000\u0000\u0000\u0148\u0149\u0001\u0000\u0000\u0000"+
		"\u0149\u0147\u0001\u0000\u0000\u0000\u0149\u014a\u0001\u0000\u0000\u0000"+
		"\u014a\u014c\u0001\u0000\u0000\u0000\u014b\u00db\u0001\u0000\u0000\u0000"+
		"\u014b\u00e6\u0001\u0000\u0000\u0000\u014b\u00f8\u0001\u0000\u0000\u0000"+
		"\u014b\u010e\u0001\u0000\u0000\u0000\u014b\u011d\u0001\u0000\u0000\u0000"+
		"\u014b\u0131\u0001\u0000\u0000\u0000\u014cH\u0001\u0000\u0000\u0000\u014d"+
		"\u014f\u0007\n\u0000\u0000\u014e\u014d\u0001\u0000\u0000\u0000\u014f\u0150"+
		"\u0001\u0000\u0000\u0000\u0150\u014e\u0001\u0000\u0000\u0000\u0150\u0151"+
		"\u0001\u0000\u0000\u0000\u0151\u0152\u0001\u0000\u0000\u0000\u0152\u0153"+
		"\u0006$\u0000\u0000\u0153J\u0001\u0000\u0000\u0000\u0154\u0155\u0005/"+
		"\u0000\u0000\u0155\u0156\u0005/\u0000\u0000\u0156\u015a\u0001\u0000\u0000"+
		"\u0000\u0157\u0159\t\u0000\u0000\u0000\u0158\u0157\u0001\u0000\u0000\u0000"+
		"\u0159\u015c\u0001\u0000\u0000\u0000\u015a\u015b\u0001\u0000\u0000\u0000"+
		"\u015a\u0158\u0001\u0000\u0000\u0000\u015b\u015d\u0001\u0000\u0000\u0000"+
		"\u015c\u015a\u0001\u0000\u0000\u0000\u015d\u015e\u0005\n\u0000\u0000\u015e"+
		"\u015f\u0001\u0000\u0000\u0000\u015f\u0160\u0006%\u0000\u0000\u0160L\u0001"+
		"\u0000\u0000\u0000\u0161\u0162\u0005/\u0000\u0000\u0162\u0163\u0005*\u0000"+
		"\u0000\u0163\u0167\u0001\u0000\u0000\u0000\u0164\u0166\t\u0000\u0000\u0000"+
		"\u0165\u0164\u0001\u0000\u0000\u0000\u0166\u0169\u0001\u0000\u0000\u0000"+
		"\u0167\u0168\u0001\u0000\u0000\u0000\u0167\u0165\u0001\u0000\u0000\u0000"+
		"\u0168\u016a\u0001\u0000\u0000\u0000\u0169\u0167\u0001\u0000\u0000\u0000"+
		"\u016a\u016b\u0005*\u0000\u0000\u016b\u016c\u0005/\u0000\u0000\u016c\u016d"+
		"\u0001\u0000\u0000\u0000\u016d\u016e\u0006&\u0000\u0000\u016eN\u0001\u0000"+
		"\u0000\u0000 \u0000\u00c0\u00c7\u00cf\u00d6\u00d8\u00dd\u00e3\u00ea\u00ee"+
		"\u00f3\u00f5\u00fa\u0100\u0103\u0107\u010c\u0114\u011a\u0124\u0128\u012d"+
		"\u012f\u0137\u013d\u0140\u0144\u0149\u014b\u0150\u015a\u0167\u0001\u0006"+
		"\u0000\u0000";
	public static final ATN _ATN =
		new ATNDeserializer().deserialize(_serializedATN.toCharArray());
	static {
		_decisionToDFA = new DFA[_ATN.getNumberOfDecisions()];
		for (int i = 0; i < _ATN.getNumberOfDecisions(); i++) {
			_decisionToDFA[i] = new DFA(_ATN.getDecisionState(i), i);
		}
	}
}