
import random
from enum import Enum

class DataBoundary(Enum):
    TINYINT_BOUNDARY = [-128, 127]
    SMALLINT_BOUNDARY = [-32768, 32767]
    INT_BOUNDARY = [-2147483648, 2147483647]
    BIGINT_BOUNDARY = [-9223372036854775808, 9223372036854775807]
    UTINYINT_BOUNDARY = [0, 255]
    USMALLINT_BOUNDARY = [0, 65535]
    UINT_BOUNDARY = [0, 4294967295]
    UBIGINT_BOUNDARY = [0, 18446744073709551615]
    FLOAT_BOUNDARY = [-3.40E+38, 3.40E+38]
    DOUBLE_BOUNDARY = [-1.7e+308, 1.7e+308]
    BOOL_BOUNDARY = [True, False]
    # GEOMETRY_BOUNDARY = ["point(1.0 1.0)", "LINESTRING(1.0 1.0, 2.0 2.0)", "POLYGON((1.0 1.0, 2.0 2.0, 1.0 1.0))"]
    GEOMETRY_BOUNDARY = ["point(1.0 1.0)"]
    BINARY_MAX_LENGTH = 65517
    NCHAR_MAX_LENGTH = 16379
    DBNAME_MAX_LENGTH = 64
    STBNAME_MAX_LENGTH = 192
    TBNAME_MAX_LENGTH = 192
    CHILD_TBNAME_MAX_LENGTH = 192
    TAG_KEY_MAX_LENGTH = 64
    COL_KEY_MAX_LENGTH = 64
    MAX_TAG_COUNT = 128
    MAX_TAG_COL_COUNT = 4096
    mnodeShmSize = [6292480, 2147483647]
    mnodeShmSize_default = 6292480
    vnodeShmSize = [6292480, 2147483647]
    vnodeShmSize_default = 31458304
    DB_PARAM_BUFFER_CONFIG = {"create_name": "buffer", "query_name": "buffer", "vnode_json_key": "szBuf", "boundary": [3, 16384], "default": 256}
    DB_PARAM_CACHELAST_CONFIG = {"create_name": "cachemodel", "query_name": "cachemodel", "vnode_json_key": "cacheLast", "boundary": {None:0, 'last_row':1, 'last_value':2, 'both':3}, "default": {None:0}}
    DB_PARAM_COMP_CONFIG = {"create_name": "comp", "query_name": "compression", "vnode_json_key": "", "boundary": [0, 1, 2], "default": 2}
    DB_PARAM_DURATION_CONFIG = {"create_name": "duration", "query_name": "duration", "vnode_json_key": "daysPerFile", "boundary": [1, 3650, '60m', '5256000m', '1h', '87600h', '1d', '3650d'], "default": "14400m"}
    DB_PARAM_FSYNC_CONFIG = {"create_name": "wal_fsync_period", "query_name": "wal_fsync_period", "vnode_json_key": "wal.fsyncPeriod", "boundary": [0, 180000], "default": 3000}
    DB_PARAM_KEEP_CONFIG = {"create_name": "keep", "query_name": "keep", "vnode_json_key": "", "boundary": [1, 365000,'1440m','525600000m','24h','8760000h','1d','365000d'], "default": "5256000m,5256000m,5256000m"}
    DB_PARAM_MAXROWS_CONFIG = {"create_name": "maxrows", "query_name": "maxrows", "vnode_json_key": "maxRows", "boundary": [200, 10000000], "default": 4096}
    DB_PARAM_MINROWS_CONFIG = {"create_name": "minrows", "query_name": "minrows", "vnode_json_key": "minRows", "boundary": [10, 1000000], "default": 100}
    DB_PARAM_NTABLES_CONFIG = {"create_name": "ntables", "query_name": "ntables", "vnode_json_key": "", "boundary": 0, "default": 0}
    DB_PARAM_PAGES_CONFIG = {"create_name": "pages", "query_name": "pages", "vnode_json_key": "szCache", "boundary": [64], "default": 256}
    DB_PARAM_PAGESIZE_CONFIG = {"create_name": "pagesize", "query_name": "pagesize", "vnode_json_key": "szPage", "boundary": [1, 16384], "default": 4}
    DB_PARAM_PRECISION_CONFIG = {"create_name": "precision", "query_name": "precision", "vnode_json_key": "", "boundary": ['ms', 'us', 'ns'], "default": "ms"}
    DB_PARAM_REPLICA_CONFIG = {"create_name": "replica", "query_name": "replica", "vnode_json_key": "syncCfg.replicaNum", "boundary": [1], "default": 1}
    DB_PARAM_SINGLE_STABLE_CONFIG = {"create_name": "single_stable", "query_name": "single_stable", "vnode_json_key": "", "boundary": [0, 1], "default": 0}
    DB_PARAM_STRICT_CONFIG = {"create_name": "strict", "query_name": "strict", "vnode_json_key": "", "boundary": {"off": "off", "on": "on"}, "default": "off"}
    DB_PARAM_VGROUPS_CONFIG = {"create_name": "vgroups", "query_name": "vgroups", "vnode_json_key": "", "boundary": [1, 1024], "default": 2}
    DB_PARAM_WAL_CONFIG = {"create_name": "wal_level", "query_name": "wal_level", "vnode_json_key": "wal.level", "boundary": [0, 2], "default": 1}
    SAMPLE_BOUNDARY = [1, 1000]
    TIMEZONE_BOUNDARY = [0, 1]
    HISTOGRAM_BOUNDARY = [0, 1]
    IGNORE_NEGATIVE_BOUNDARY = [0, 1]
    DIFF_IGNORE_BOUNDARY = [0, 1, 2, 3]
    PERCENTILE_BOUNDARY = [1, 10]
    LEASTSQUARES_BOUNDARY = [1, 10]
    SUBSTR_BOUNDARY = [1, 10]
    TAIL_BOUNDARY = [1, 100]
    TAIL_OFFSET_BOUNDARY = [0, 100]
    TOP_BOUNDARY = [1, 100]
    MAVG_BOUNDARY = [1, 1000]
    CONCAT_BOUNDARY = [2, 8]
    TIME_UNIT = ['b', 'u', 'a', 's', 'm', 'h', 'd', 'w']
    STATECOUNT_UNIT = ["LT", "LE", "EQ", "NE", "GE", "GT"]
    TO_CHAR_UNIT = ['AM,am,PM,pm', 'A.M.,a.m.,P.M.,p.m.', 'YYYY,yyyy', 'YYY,yyy', 'YY,yy', 'Y,y','MONTH', 'Month',
                    'month', 'MON', 'Mon', 'mon', 'MM,mm', 'DD,dd', 'DAY', 'Day', 'day', 'DY', 'Dy', 'dy', 'DDD',
                    'D,d', 'HH24,hh24', 'hh12,HH12, hh, HH', 'MI,mi', 'SS,ss', 'MS,ms', 'US,us', 'NS,ns', 'TZH,tzh']
    ALL_TYPE_UNIT = ['TINYINT', 'SMALLINT', 'INT', 'BIGINT', 'TINYINT UNSIGNED', 'SMALLINT UNSIGNED', 'INT UNSIGNED', 'BIGINT UNSIGNED', 'FLOAT', 'DOUBLE', 'BINARY', 'VARCHAR', 'VARBINARY', 'NCHAR', 'BOOL', 'TIMESTAMP', 'GEOMETRY(64)']
    WINDOW_UNIT = ['INTERVAL', 'SESSION', 'STATE_WINDOW', 'COUNT_WINDOW', 'EVENT_WINDOW']
    FILL_UNIT = ["NULL", "PREV", "NEXT", "LINEAR", "VALUE, 0", "NULL_F", "VALUE_F, 0"]

class FunctionMap(Enum):
    # TODO TO_JSON
    NUMERIC = {
        'types': ['TINYINT', 'SMALLINT', 'INT', 'BIGINT', 'TINYINT UNSIGNED', 'SMALLINT UNSIGNED', 'INT UNSIGNED', 'BIGINT UNSIGNED', 'FLOAT', 'DOUBLE'],
        'mathFuncs': ['ABS', 'ACOS', 'ASIN', 'ATAN', 'CEIL', 'COS', 'FLOOR', 'LOG', 'POW', 'ROUND', 'SIN', 'SQRT', 'TAN'],
        'strFuncs': [],
        'timeFuncs': ['NOW', 'TIMEZONE', 'TODAY'],
        'aggFuncs': ['APERCENTILE', 'AVG', 'COUNT', 'LEASTSQUARES', 'SPREAD', 'STDDEV', 'SUM', 'HYPERLOGLOG', 'PERCENTILE'],
        'selectFuncs': ['FIRST', 'LAST', 'LAST_ROW', 'MAX', 'MIN', 'MODE'],
        'specialFuncs': ['IRATE', 'TWA'],
        'VariableFuncs': ['BOTTOM', 'INTERP', 'UNIQUE', 'TOP', 'TAIL', 'SAMPLE', 'DIFF', 'CSUM', 'MAVG', 'DERIVATIVE', 'STATECOUNT', 'STATEDURATION', 'HISTOGRAM'],
        'castFuncs': ['CAST', 'TO_ISO8601'],
        'castTypes': ['TINYINT', 'SMALLINT', 'INT', 'BIGINT', 'TINYINT UNSIGNED', 'SMALLINT UNSIGNED', 'INT UNSIGNED', 'BIGINT UNSIGNED', 'FLOAT', 'DOUBLE', 'BINARY', 'VARCHAR', 'NCHAR', 'BOOL', 'TIMESTAMP', 'GEOMETRY(64)']
    }
    TEXT = {
        'types': ['BINARY', 'VARCHAR', 'NCHAR'],
        'mathFuncs': [],
        'strFuncs': ['CHAR_LENGTH', 'CONCAT', 'CONCAT_WS', 'LENGTH', 'LOWER', 'LTRIM', 'RTRIM', 'SUBSTR', 'UPPER'],
        'timeFuncs': ['NOW', 'TIMETRUNCATE', 'TIMEZONE', 'TODAY'],
        'aggFuncs': ['COUNT', 'HYPERLOGLOG'],
        'selectFuncs': ['FIRST', 'LAST', 'LAST_ROW', 'MODE'],
        'specialFuncs': [],
        'VariableFuncs': ['BOTTOM', 'INTERP', 'UNIQUE', 'TOP', 'TAIL', 'SAMPLE'],
        'castFuncs': ['CAST', 'TO_UNIXTIMESTAMP'],
        'castTypes': DataBoundary.ALL_TYPE_UNIT.value
    }
    BINARY = {
        'types': ['VARBINARY'],
        'mathFuncs': [],
        'strFuncs': ['LENGTH'],
        'timeFuncs': ['NOW', 'TIMETRUNCATE', 'TIMEZONE', 'TODAY'],
        'aggFuncs': ['COUNT', 'HYPERLOGLOG'],
        'selectFuncs': ['FIRST', 'LAST', 'LAST_ROW', 'MODE'],
        'specialFuncs': [],
        'VariableFuncs': ['UNIQUE', 'TAIL', 'SAMPLE'],
        'castFuncs': [],
        'castTypes': []
    }
    BOOLEAN = {
        'types': ['BOOL'],
        'mathFuncs': [],
        'strFuncs': [],
        'timeFuncs': ['NOW'],
        'aggFuncs': ['COUNT', 'HYPERLOGLOG'],
        'selectFuncs': ['FIRST', 'LAST', 'LAST_ROW', 'MODE'],
        'specialFuncs': [],
        'VariableFuncs': ['UNIQUE', 'TAIL', 'SAMPLE'],
        'castFuncs': ['CAST'],
        'castTypes': ['TINYINT', 'SMALLINT', 'INT', 'BIGINT', 'TINYINT UNSIGNED', 'SMALLINT UNSIGNED', 'INT UNSIGNED', 'BIGINT UNSIGNED', 'FLOAT', 'DOUBLE', 'BINARY', 'VARCHAR', 'NCHAR', 'BOOL', 'TIMESTAMP', 'GEOMETRY(64)']
    }
    TIMESTAMP = {
        'types': ['TIMESTAMP'],
        'mathFuncs': [],
        'strFuncs': [],
        'timeFuncs': ['NOW', 'TIMEDIFF', 'TIMETRUNCATE', 'TIMEZONE', 'TODAY'],
        'aggFuncs': ['ELAPSED', 'SPREAD'],
        'selectFuncs': ['FIRST', 'LAST', 'LAST_ROW', 'MODE'],
        'specialFuncs': [],
        'VariableFuncs': ['UNIQUE', 'SAMPLE'],
        'castFuncs': ['CAST', 'TO_ISO8601', 'TO_CHAR'],
        'castTypes': ['TINYINT', 'SMALLINT', 'INT', 'BIGINT', 'TINYINT UNSIGNED', 'SMALLINT UNSIGNED', 'INT UNSIGNED', 'BIGINT UNSIGNED', 'FLOAT', 'DOUBLE', 'BINARY', 'VARCHAR', 'NCHAR', 'BOOL', 'TIMESTAMP', 'GEOMETRY(64)']
    }


class SQLLancer:
    def formatTimediff(self, expr1):
        # 1b(纳秒), 1u(微秒)，1a(毫秒)，1s(秒)，1m(分)，1h(小时)，1d(天), 1w(周)
        time_unit = random.choice(DataBoundary.TIME_UNIT.value)
        expr2 = f'{expr1}+1{time_unit}'
        return f"TIMEDIFF({expr1}, {expr2})"

    def formatDiff(self, expr1):
        ignore_val = random.choice(DataBoundary.DIFF_IGNORE_BOUNDARY.value)
        if ignore_val == 0:
            return f"DIFF({expr1})"
        else:
            return f"DIFF({expr1}, {ignore_val})"

    def formatTimeTruncate(self, expr):
        time_unit = random.choice(DataBoundary.TIME_UNIT.value[2:])
        use_current_timezone = random.choice(DataBoundary.TIMEZONE_BOUNDARY.value)
        return f'TIMETRUNCATE({expr}, 1{time_unit}, {use_current_timezone})'

    def formatHistogram(self, expr):
        user_input1 = [f'HISTOGRAM({expr}, "user_input", "[1, 3, 5, 7]", {random.choice([0, 1])})']
        linear_bin  = [f'HISTOGRAM({expr}, "linear_bin", \'{{"start": 0.0, "width": 5.0, "count": 5, "infinity": true}}\', {random.choice(DataBoundary.HISTOGRAM_BOUNDARY.value)})']
        user_input2 = [f'HISTOGRAM({expr}, "user_input", \'{{"start":1.0, "factor": 2.0, "count": 5, "infinity": true}}\', {random.choice(DataBoundary.HISTOGRAM_BOUNDARY.value)})']
        funcList = user_input1 + linear_bin + user_input2
        return random.choice(funcList)

    def formatPercentile(self, expr):
        rcnt = random.randint(DataBoundary.PERCENTILE_BOUNDARY.value[0], DataBoundary.PERCENTILE_BOUNDARY.value[1])
        p = random.sample(range(DataBoundary.PERCENTILE_BOUNDARY.value[0], DataBoundary.PERCENTILE_BOUNDARY.value[1]*10), rcnt)
        return f'PERCENTILE({expr},{",".join(map(str, p))})'

    def formatStatecount(self, expr):
        val = random.randint(DataBoundary.PERCENTILE_BOUNDARY.value[0], DataBoundary.PERCENTILE_BOUNDARY.value[1])
        oper = random.choice(DataBoundary.STATECOUNT_UNIT.value)
        return f'STATECOUNT({expr}, "{oper}", {val})'

    def formatStateduration(self, expr):
        val = random.randint(DataBoundary.PERCENTILE_BOUNDARY.value[0], DataBoundary.PERCENTILE_BOUNDARY.value[1])
        oper = random.choice(DataBoundary.STATECOUNT_UNIT.value)
        unit = random.choice(DataBoundary.TIME_UNIT.value)
        return f'STATECOUNT({expr}, "{oper}", {val}, 1{unit})'

    def formatConcat(self, expr, *args):
        print("---1args:", *args)
        print("---2args:", args)
        base = f'CONCAT("pre_", cast({expr} as nchar({DataBoundary.CONCAT_BOUNDARY.value[1]})))'
        argsVals = list()
        for i in range(len(args[:DataBoundary.CONCAT_BOUNDARY.value[1]-1])):
            print("---args[i]:", args[i])
            argsVals.append(f'cast({args[i]} as nchar({DataBoundary.CONCAT_BOUNDARY.value[1]}))')
        if len(argsVals) == 0:
            return base
        else:
            return f'CONCAT({base}, {", ".join(argsVals)})'

    def formatConcatWs(self, expr, *args):
        separator_expr = random.choice([",", ":", ";", "_", "-"])
        base = f'CONCAT_WS("{separator_expr}", "pre_", cast({expr} as nchar({DataBoundary.CONCAT_BOUNDARY.value[1]})))'
        print("----base", base)
        argsVals = list()
        for i in range(len(args[:DataBoundary.CONCAT_BOUNDARY.value[1]-1])):
            argsVals.append(f'cast({args[i]} as nchar({DataBoundary.CONCAT_BOUNDARY.value[1]}))')
            print("----argsVals", argsVals)
        if len(argsVals) == 0:
            return base
        else:
            return f'CONCAT_WS("{separator_expr}", {base}, {", ".join(argsVals)})'

    def formatSubstr(self, expr):
        pos = random.choice(DataBoundary.SUBSTR_BOUNDARY.value)
        length = random.choice(DataBoundary.SUBSTR_BOUNDARY.value)
        return f'SUBSTR({expr}, {pos}, {length})'

    def formatCast(self, expr, castTypeList):
        return f'CAST({expr} AS {random.choice(castTypeList)})'

    def formatFunc(self, func, colname, castTypeList="nchar", *args, **kwarg):
        if func in ['ABS', 'ACOS', 'ASIN', 'ATAN', 'CEIL', 'COS', 'FLOOR', 'LOG', 'ROUND', 'SIN', 'SQRT', 'TAN'] + \
                    ['AVG', 'COUNT', 'SPREAD', 'STDDEV', 'SUM', 'HYPERLOGLOG'] + \
                    ['FIRST', 'LAST', 'LAST_ROW', 'MAX', 'MIN', 'MODE', 'UNIQUE'] + \
                    ['CSUM', 'IRATE', 'TWA'] + \
                    ['CHAR_LENGTH', 'LENGTH', 'LOWER', 'LTRIM', 'RTRIM', 'UPPER', 'TO_JSON']:
            return f"{func}({colname})"
        elif func in ['NOW', 'TODAY', 'TIMEZONE', 'DATABASES', 'CLIENT_VERSION', 'SERVER_VERSION', 'SERVER_STATUS', 'CURRENT_USER']:
            return f"{func}()"
        elif func in ['TIMEDIFF']:
            return self.formatTimediff(colname)
        elif func in ['TIMEDIFF']:
            return self.formatDiff(colname)
        elif func in ['TIMETRUNCATE']:
            return self.formatTimeTruncate(colname)
        elif func in ['APERCENTILE']:
            return f'{func}({colname}, {random.randint(DataBoundary.PERCENTILE_BOUNDARY.value[0], DataBoundary.PERCENTILE_BOUNDARY.value[1])}, "{random.choice(["default", "t-digest"])}")'
        elif func in ['LEASTSQUARES']:
            return f"{func}({colname}, {random.randint(1, DataBoundary.LEASTSQUARES_BOUNDARY.value[1])}, {random.randint(1, DataBoundary.LEASTSQUARES_BOUNDARY.value[1])})"
        elif func in ['HISTOGRAM']:
            return self.formatHistogram(colname)
        elif func in ['PERCENTILE']:
            return self.formatPercentile(colname)
        elif func in ['ELAPSED']:
            return f"{func}({colname}, 1{random.choice(DataBoundary.TIME_UNIT.value)})"
        elif func in ['POW']:
            return f"{func}({colname}, {random.choice(DataBoundary.SAMPLE_BOUNDARY.value)})"
        elif func in ['INTERP', 'DIFF', 'TO_UNIXTIMESTAMP']:
            return f"{func}({colname}, {random.choice(DataBoundary.IGNORE_NEGATIVE_BOUNDARY.value)})"
        elif func in ['SAMPLE']:
            return f"{func}({colname}, {random.choice(DataBoundary.SAMPLE_BOUNDARY.value)})"
        elif func in ['TAIL']:
            return f"{func}({colname}, {random.choice(DataBoundary.TAIL_BOUNDARY.value)}, {random.choice(DataBoundary.TAIL_OFFSET_BOUNDARY.value)})"
        elif func in ['TOP', 'BOTTOM']:
            return f"{func}({colname}, {random.choice(DataBoundary.TOP_BOUNDARY.value)})"
        elif func in ['DERIVATIVE']:
            return f"{func}({colname}, 1{random.choice(DataBoundary.TIME_UNIT.value[3:])}, {random.choice(DataBoundary.IGNORE_NEGATIVE_BOUNDARY.value)})"
        elif func in ['MAVG']:
            return f"{func}({colname}, {random.choice(DataBoundary.MAVG_BOUNDARY.value)})"
        elif func in ['STATECOUNT']:
            return self.formatStatecount(colname)
        elif func in ['STATEDURATION']:
            return self.formatStateduration(colname)
        elif func in ['CONCAT']:
            print("----args", args)
            return self.formatConcat(colname, *args)
        elif func in ['CONCAT_WS']:
            return self.formatConcatWs(colname, *args)
        elif func in ['SUBSTR']:
            return self.formatSubstr(colname, *args)
        elif func in ['CAST']:
            return self.formatCast(colname, castTypeList)
        elif func in ['TO_ISO8601']:
            timezone = random.choice(["+00:00", "-00:00", "+08:00", "-08:00", "+12:00", "-12:00"])
            return f'{func}({colname}, "{timezone}")'
        elif func in ['TO_CHAR', 'TO_TIMESTAMP']:
            return f'{func}({colname}, "{random.choice(DataBoundary.TO_CHAR_UNIT.value)}")'
        else:
            pass

    def setGroupTag(self, fm, funcList):
        """
        Check if there are common elements between `funcList` and `selectFuncs` in `fm`.

        Args:
            fm (dict): The dictionary containing the 'selectFuncs' key.
            funcList (list): The list of functions to compare with 'selectFuncs'.

        Returns:
            bool: True if there are common elements, False otherwise.
        """
        selectFuncs = fm['selectFuncs']
        s1 = set(funcList)
        s2 = set(selectFuncs)
        common_elements = s1 & s2
        if len(common_elements) > 1:
            return True
        else:
            return False

    def selectFuncsFromType(self, fm, colname, doAggr):
        if doAggr == 0:
            categoryList = ['aggFuncs']
        elif doAggr == 1:
            categoryList = ['mathFuncs', 'strFuncs', 'timeFuncs', 'selectFuncs', 'castFuncs']
        elif doAggr == 2:
            categoryList = ['VariableFuncs']
        elif doAggr == 3:
            categoryList = ['specialFuncs']
        else:
            pass
        funcList = list()
        print("----categoryList", categoryList)
        print("----fm", fm)

        for category in categoryList:
            funcList += fm[category]
        print("----funcList", funcList)
        selectItems = random.sample(funcList, random.randint(1, len(funcList))) if len(funcList) > 0 else list()
        funcStrList = list()
        for func in selectItems:
            print("----func", func)
            funcStr = self.formatFunc(func, colname, fm["castTypes"])
            print("----funcStr", funcStr)
            funcStrList.append(funcStr)
        print("-------funcStrList:", funcStrList)
        print("-------funcStr:", ",".join(funcStrList))
        print("----selectItems", selectItems)
        groupKey = colname if self.setGroupTag(fm, funcList) else ""
        if doAggr == 2:
            return ",".join([random.choice(funcStrList)]), groupKey
        return ",".join(funcStrList), groupKey


        # # colTypes = ['NUMERIC', 'TEXT', 'BINARY', 'BOOLEAN', 'TIMESTAMP'...]
        # colTypes = [member.name for member in FunctionMap]
        # # cateDict = {'NUMERIC': '......', 'TEXT': '......', 'BINARY': '......', 'BOOLEAN': '......', 'TIMESTAMP': '......'...}
        # print(fm)
        # cateDict = fm.value
        # # categories = ['mathFuncs', 'strFuncs', 'timeFuncs', 'aggFuncs', 'selectFuncs', 'specialFuncs']
        # categories = [key for key in cateDict.keys() if "Funcs" in key]
        # for category in categories:
        #     if FunctionMap[category]:
        #         func = random.choice()
        #         pass
        '''
    fm:
        {
            'types': ['TINYINT', 'SMALLINT', 'INT', 'BIGINT', 'TINYINT UNSIGNED', 'SMALLINT UNSIGNED', 'INT UNSIGNED', 'BIGINT UNSIGNED', 'FLOAT', 'DOUBLE'],
            'mathFuncs': ['ABS', 'ACOS', 'ASIN', 'ATAN', 'CEIL', 'COS', 'FLOOR', 'LOG', 'POW', 'ROUND', 'SIN', 'SQRT', 'TAN'],
            'strFuncs': [],
            'timeFuncs': ['NOW', 'TIMEDIFF', 'TIMEZONE', 'TODAY'],
            'aggFuncs': ['APERCENTILE', 'AVG', 'COUNT', 'LEASTSQUARES', 'SPREAD', 'STDDEV', 'SUM', 'HYPERLOGLOG', 'HISTOGRAM', 'PERCENTILE'],
            'selectFuncs': ['BOTTOM', 'FIRST', 'INTERP', 'LAST', 'LAST_ROW', 'MAX', 'MIN', 'MODE', 'SAMPLE', 'TAIL', 'TOP', 'UNIQUE'],
            'specialFuncs': ['CSUM', 'DERIVATIVE', 'DIFF', 'IRATE', 'MAVG', 'STATECOUNT', 'STATEDURATION', 'TWA'],
            'castFuncs': ['CAST', 'TO_ISO8601'],
            'castTypes': ['TINYINT', 'SMALLINT', 'INT', 'BIGINT', 'TINYINT UNSIGNED', 'SMALLINT UNSIGNED', 'INT UNSIGNED', 'BIGINT UNSIGNED', 'FLOAT', 'DOUBLE', 'BINARY', 'VARCHAR', 'NCHAR', 'BOOL', 'TIMESTAMP']
        }
        '''
        # for category in categories:
        #     if fm[category]:
        #         func = random.choice(fm[category])
        #         if func == 'CAST':
        #             cast_type = random.choice(fm['castTypes'])
        #             return f"{func}({colname} AS {cast_type})"
        #         return f"{func}({colname})"
        # return f"COUNT({colname})"  # Default fallback

    def getFuncCategory(self, doAggr):
        if doAggr == 0:
            return "aggFuncs"
        elif doAggr == 1:
            return "nFuncs"
        else:
            return "spFuncs"

    def getRandomTimeUnitStr(self):
        """
        Generates a random time unit string.

        Returns:
            str: A string representing a random time unit.
        """
        return f'{random.randint(DataBoundary.SAMPLE_BOUNDARY.value[0], DataBoundary.SAMPLE_BOUNDARY.value[1])}{random.choice(DataBoundary.TIME_UNIT.value)}'

    def getRandomWindow(self):
        """
        Returns a random window from the available window units.

        Returns:
            str: A random window unit.
        """
        return random.choice(DataBoundary.WINDOW_UNIT.value)

    def getOffsetValue(self, rand=None):
        """
        Returns the offset value for a SQL query.

        Args:
            rand (bool, optional): If True, a random offset value will be used. If False, a time unit string will be used. Defaults to None.

        Returns:
            str: The offset value for the SQL query.
        """
        useTag = random.choice([True, False]) if rand is None else True
        offsetVal = "" if useTag else f',{self.getRandomTimeUnitStr()}'
        return offsetVal

    def getSlidingValue(self, rand=None):
        """
        Get the sliding value for the SQL query.

        Parameters:
        - rand (bool, optional): If True, use a random value for the sliding value. If False, use a predefined value. Defaults to None.

        Returns:
        - slidingVal (str): The sliding value for the SQL query.
        """
        useTag = random.choice([True, False]) if rand is None else True
        slidingVal = "" if useTag else f'SLIDING({self.getRandomTimeUnitStr()})'
        return slidingVal

    def getFillValue(self, rand=None):
        """
        Returns a fill value for SQL queries.

        Parameters:
        - rand (bool, optional): If True, a random fill value will be used. If False, an empty string will be returned. Defaults to None.

        Returns:
        - str: The fill value for SQL queries.
        """
        useTag = random.choice([True, False]) if rand is None else True
        fillVal = "" if useTag else f'FILL({random.choice(DataBoundary.FILL_UNIT.value)})'
        return fillVal

    def getWindowStr(self, window):
        if window == "INTERVAL":
            return f"{window}({self.getRandomTimeUnitStr()}{self.getOffsetValue()})"
        elif window == "SESSION":
            return 1
        elif window == "STATE_WINDOW":
            return 1
        elif window == "COUNT_WINDOW":
            return 1
        elif window == "EVENT_WINDOW":
            return 1
        else:
            return 1

    def generateRandomSql(self, colDict, tbname):
        selectPartList = []
        groupKeyList = []
        # colTypes = ['NUMERIC', 'TEXT', 'BINARY', 'BOOLEAN', 'TIMESTAMP'...]
        colTypes = [member.name for member in FunctionMap]
        print("-----colTypes", colTypes)
        doAggr = random.choice([0, 1, 2, 3])

        for column_name, column_type in colDict.items():
            print(f"-----column_name, column_type: {column_name}, {column_type}")
            for fm in FunctionMap:
                if column_type in fm.value['types']:
                    # cateDict = {'NUMERIC': '......', 'TEXT': '......', 'BINARY': '......', 'BOOLEAN': '......', 'TIMESTAMP': '......'...}
                    # cateDict = fm.value
                    # print("-----cateDict", cateDict)
                    # categories = ['mathFuncs', 'strFuncs', 'timeFuncs', 'aggFuncs', 'selectFuncs', 'specialFuncs']
                    # categories = [key for key in cateDict.keys() if "Funcs" in key]
                    # print("-----categories", categories)
                    # / selectCate = 'mathFuncs'
                    # selectCate = random.choice(categories)
                    # print("-----selectCate", selectCate)
                    selectStrs, groupKey = self.selectFuncsFromType(fm.value, column_name, doAggr)
                    print("-----selectStrs", selectStrs)
                    if len(selectStrs) > 0:
                        selectPartList.append(selectStrs)
                    print("-----selectPartList", selectPartList)
                    if len(groupKey) > 0:
                        groupKeyList.append(groupKey)
                    
                    # / funcs = ['ABS', 'ACOS', 'ASIN'....]
                    # funcs = random.sample(fm.value[selectFunc], random.randint(1, len(fm.value[selectFunc])))
                    # print("-----funcs", funcs)
                    print("\n")
                    # if func == 'CAST':
                    #     cast_type = random.choice(FunctionMap['castTypes'])
                    #     func_expression = f"{func}({column_name} AS {cast_type})"
                    #     select_parts.append(func_expression)
                    # else:
                    #     func_expression = f"{func}({column_name})"
                    #     select_parts.append(func_expression)
                    # pass
                    # func_expression = self.selectFuncsFromType(fm.value, column_name)
                    # select_parts.append(func_expression)
        if doAggr == 2:
            selectPartList = [random.choice(selectPartList)]
        if len(groupKeyList) > 0:
            groupKeyStr = ",".join(groupKeyList)
            return f"SELECT {', '.join(selectPartList)} FROM {tbname} GROUP BY {groupKeyStr};"
        return f"SELECT {', '.join(selectPartList)} FROM {tbname};"

sqllancer = SQLLancer()
colDict = {"ts": "TIMESTAMP", "c1": "INT", "c2": "NCHAR", "c3": "BOOL"}
tbname = "tb"
# print(sqllancer.formatConcat("c1", "c2", "c3"))
randomSql = sqllancer.generateRandomSql(colDict, tbname)
print(randomSql)


    # def generateQueries_n(self, dbc: DbConn, selectItems) -> List[SqlQuery]:
    #     ''' Generate queries to test/exercise this super table '''
    #     ret = []  # type: List[SqlQuery]


    #     for rTbName in self.getRegTables(dbc):  # regular tables

    #         filterExpr = Dice.choice([  # TODO: add various kind of WHERE conditions
    #             None
    #         ])

    #         # Run the query against the regular table first
    #         doAggr = (Dice.throw(2) == 0)  # 1 in 2 chance
    #         if not doAggr:  # don't do aggregate query, just simple one
    #             query_parts = []
    #             for colName, colType in selectItems.items():
    #                 for groupKey, group in FunctionMap:
    #                     query_parts.append(colName)

    #             commonExpr = Dice.choice([
    #                 '*',
    #                 'abs(speed)',
    #                 'acos(speed)',
    #                 'asin(speed)',
    #                 'atan(speed)',
    #                 'ceil(speed)',
    #                 'cos(speed)',
    #                 'cos(speed)',
    #                 'floor(speed)',
    #                 'log(speed,2)',
    #                 'pow(speed,2)',
    #                 'round(speed)',
    #                 'sin(speed)',
    #                 'sqrt(speed)',
    #                 'char_length(color)',
    #                 'concat(color,color)',
    #                 'concat_ws(" ", color,color," ")',
    #                 'length(color)',
    #                 'lower(color)',
    #                 'ltrim(color)',
    #                 'substr(color , 2)',
    #                 'upper(color)',
    #                 'cast(speed as double)',
    #                 'cast(ts as bigint)',
    #                 # 'TO_ISO8601(color)',
    #                 # 'TO_UNIXTIMESTAMP(ts)',
    #                 'now()',
    #                 'timediff(ts,now)',
    #                 'timezone()',
    #                 'TIMETRUNCATE(ts,1s)',
    #                 'TIMEZONE()',
    #                 'TODAY()',
    #                 'distinct(color)'
    #             ]
    #             )
    #             ret.append(SqlQuery(  # reg table
    #                 "select {} from {}.{}".format(commonExpr, self._dbName, rTbName)))
    #             ret.append(SqlQuery(  # super table
    #                 "select {} from {}.{}".format(commonExpr, self._dbName, self.getName())))
    #         else:  # Aggregate query
    #             aggExpr = Dice.choice([
    #                 'count(*)',
    #                 'avg(speed)',
    #                 # 'twa(speed)', # TODO: this one REQUIRES a where statement, not reasonable
    #                 'sum(speed)',
    #                 'stddev(speed)',
    #                 # SELECTOR functions
    #                 'min(speed)',
    #                 'max(speed)',
    #                 'first(speed)',
    #                 'last(speed)',
    #                 'top(speed, 50)',  # TODO: not supported?
    #                 'bottom(speed, 50)',  # TODO: not supported?
    #                 'apercentile(speed, 10)',  # TODO: TD-1316
    #                 'last_row(*)',  # TODO: commented out per TD-3231, we should re-create
    #                 # Transformation Functions
    #                 # 'diff(speed)', # TODO: no supported?!
    #                 'spread(speed)',
    #                 'elapsed(ts)',
    #                 'mode(speed)',
    #                 'bottom(speed,1)',
    #                 'top(speed,1)',
    #                 'tail(speed,1)',
    #                 'unique(color)',
    #                 'csum(speed)',
    #                 'DERIVATIVE(speed,1s,1)',
    #                 'diff(speed,1)',
    #                 'irate(speed)',
    #                 'mavg(speed,3)',
    #                 'sample(speed,5)',
    #                 'STATECOUNT(speed,"LT",1)',
    #                 'STATEDURATION(speed,"LT",1)',
    #                 'twa(speed)'

    #             ])  # TODO: add more from 'top'

    #             # if aggExpr not in ['stddev(speed)']: # STDDEV not valid for super tables?! (Done in TD-1049)
    #             sql = "select {} from {}.{}".format(aggExpr, self._dbName, self.getName())
    #             if Dice.throw(3) == 0:  # 1 in X chance
    #                 partion_expr = Dice.choice(['color', 'tbname'])
    #                 sql = sql + ' partition BY ' + partion_expr + ' order by ' + partion_expr
    #                 Progress.emit(Progress.QUERY_GROUP_BY)
    #                 # Logging.info("Executing GROUP-BY query: " + sql)
    #             ret.append(SqlQuery(sql))

    #     return ret

