#include "DRE_Master.h"

///
/// \brief Master状态转化为string输出
/// \param state
/// \return
///
QString DRE_Master::Master_stateToString(int state)
{
    QString str;
    switch(state){
    case STATE_NONE:
        str = "STATE_NONE";
        break;
    case STATE_INIT:
         str = "STATE_INIT";
        break;
    case STATE_PRE_OP:
         str = "STATE_PRE_OP";
        break;
    case STATE_BOOT:
         str = "STATE_BOOT";
        break;
    case STATE_SAFE_OP:
         str = "STATE_SAFE_OP";
        break;
    case STATE_OPERATIONAL:
         str = "STATE_OPERATIONAL";
        break;
    case STATE_ACK:
         str = "STATE_ACK";
        break;
    default:
         str = "STATE_UNKNOWN:0x"+QString::number(state,16);
        break;
    }

    return str;
}

/******************** Ethercat_SlaveMSG_Item **************************************/

///
/// \brief Item的状态转化为string输出
/// \return
///
const QString Ethercat_SlaveMSG_Item::dataType_Tostring()
{
    QString str = "";
    uint16_t dtype = m_date_type;

    switch(dtype)
    {
        case DRE_ECT_BOOLEAN:
            str = "BOOLEAN";
            break;
        case DRE_ECT_INTEGER8:
            str = "INTEGER8";
            break;
        case DRE_ECT_INTEGER16:
            str = "INTEGER16";
            break;
        case DRE_ECT_INTEGER32:
            str = "INTEGER32";
            break;
        case DRE_ECT_INTEGER24:
            str = "INTEGER24";
            break;
        case DRE_ECT_INTEGER64:
            str = "INTEGER64";
            break;
        case DRE_ECT_UNSIGNED8:
            str = "UNSIGNED8";
            break;
        case DRE_ECT_UNSIGNED16:
            str = "UNSIGNED16";
            break;
        case DRE_ECT_UNSIGNED32:
            str = "UNSIGNED32";
            break;
        case DRE_ECT_UNSIGNED24:
            str = "UNSIGNED24";
            break;
        case DRE_ECT_UNSIGNED64:
            str = "UNSIGNED64";
            break;
        case DRE_ECT_REAL32:
            str = "REAL32";
            break;
        case DRE_ECT_REAL64:
            str = "REAL64";
            break;
        case DRE_ECT_BIT1:
            str = "BIT1";
            break;
        case DRE_ECT_BIT2:
            str = "BIT2";
            break;
        case DRE_ECT_BIT3:
            str = "BIT3";
            break;
        case DRE_ECT_BIT4:
            str = "BIT4";
            break;
        case DRE_ECT_BIT5:
            str = "BIT5";
            break;
        case DRE_ECT_BIT6:
            str = "BIT6";
            break;
        case DRE_ECT_BIT7:
            str = "BIT7";
            break;
        case DRE_ECT_BIT8:
            str = "BIT8";
            break;
        case DRE_ECT_VISIBLE_STRING:
            str = "VISIBLE_STRING";
            break;
        case DRE_ECT_OCTET_STRING:
            str = "OCTET_STRING";
            break;
        default:
            str = "User Type 0x"+QString::number(dtype,16);
    }
    return str;
}

///
/// \brief dump数据(用于测试)
/// \param isDebug
/// \return
///
const QStringList Ethercat_SlaveMSG_Item::dump_data(bool isDebug)
{
    QStringList str_list;
    QString str;
    QString str_debug;

    str_debug = isDebug == true? "slave_num=" : "";
    str = QString::number(m_slave_num);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "sm_num=" : "";
    str = QString::number(m_sm_num);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "addr_offset=" : "";
    str = "0x"+QString::number(m_addr_offset,16);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "addr_bit=" : "";
    str = "0x"+QString::number(m_addr_bit,16);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "obj_index=" : "";
    str = "0x"+QString::number(m_obj_index,16);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "obj_index_sub_num=" : "";
    str = "0x"+QString::number(m_obj_index_sub,16);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "item_bitLen=" : "";
    str = "0x"+QString::number(m_item_bitLen,16);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "dataType=" : "";
    str = dataType_Tostring();
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "name=" : "";
    str = m_name;
    str_list.append(str_debug+str);

    return str_list;
}

/***********************  Ethercat_SlaveMSG_Item END  *********************************/

/******************** Ethercat_Slave **************************************/
///
/// \brief dump数据(用于测试)
/// \param isDebug
/// \return
///
const QStringList Ethercat_Slave::dump_data(bool isDebug)
{
    QStringList str_list;
    QString str;
    QString str_debug;

    str_debug = isDebug == true? "slave_index=" : "";
    str = QString::number(m_slave_index);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "name=" : "";
    str = m_name;
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "m_Obits=" : "";
    str = "0x"+QString::number(m_Obits,16);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "m_Ibits=" : "";
    str = "0x"+QString::number(m_Ibits,16);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "m_eep_man=" : "";
    str = "0x"+QString::number(m_eep_man,16);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "m_eep_id=" : "";
    str = "0x"+QString::number(m_eep_id,16);
    str_list.append(str_debug+str);
    str_debug = isDebug == true? "m_eep_rev=" : "";
    str = "0x"+QString::number(m_eep_rev,16);
    str_list.append(str_debug+str);

    return str_list;
}
