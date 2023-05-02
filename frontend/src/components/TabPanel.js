
interface TabPanelProps { // Struct
    children?: React.ReactNode;
    index: number;
    value: number;
  }

  /**
   * Update React.Node given prop
   * @param {TabPanelProps} props 
   */
export default function TabPanel(props: TabPanelProps) { // Render tool bar on click
    const { children, value, index, ...other } = props;

    return (
        <div role="tabpanel" hidden={value !== index} id={`simple-tabpanel-${index}`}
        aria-labelledby={`simple-tab-${index}`} 
        {...other}
        > 
        {value === index && ( children )} 
        </div>
    );
}