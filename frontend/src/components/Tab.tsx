// @ts-nocheck
import * as React from 'react';
import Tabs from '@mui/material/Tabs';
import Tab from '@mui/material/Tab';
import Typography from '@mui/material/Typography';
import Box from '@mui/material/Box';
import Button from './ReusableButton'

import { profileDataType } from '../pages/Profilepage';

interface TabPanelProps {
  children?: React.ReactNode;
  index: number;
  value: number;
}

function TabPanel(props: TabPanelProps) {
  const { children, value, index, ...other } = props;

  return (
    <div
      role="tabpanel"
      hidden={value !== index}
      id={`simple-tabpanel-${index}`}
      aria-labelledby={`simple-tab-${index}`}
      {...other}
    >
      {value === index && (
        <Box sx={{ p: 3 }}>
          <Typography>{children}</Typography>
        </Box>
      )}
    </div>
  );
}

function a11yProps(index: number) {
  return {
    id: `simple-tab-${index}`,
    'aria-controls': `simple-tabpanel-${index}`,
  };
}

export default function StatTab({ data }: { data: profileDataType}) {
  const [value, setValue] = React.useState(0);

  const handleChange = (event: React.SyntheticEvent, newValue: number) => {
    setValue(newValue);
  };

  const renderTab = (type: string) => {
    console.log(data["classic"])
    return(
      <Box display="flex">
        <Box width="400px" margin = "0 20px">
          <Typography variant="h4">Elo</Typography>
          <Typography variant="h4">Wins</Typography>
          <Typography variant="h4">Losses</Typography>
          <Typography variant="h4">Draws</Typography>
          <Button background="purple" width={400}>Elo History Chart</Button>
        </Box>
        <Box>
          <Typography variant="h4">{data[type].elo}</Typography>
          <Typography variant="h4">{data[type].wins}</Typography>
          <Typography variant="h4">{data[type].losses}</Typography>
          <Typography variant="h4">{data[type].draws}</Typography>
        </Box>
      </Box>
    )
  }

  return (
    <Box sx={{ width: '100%'}}>
      <Box sx={{ borderBottom: 1, borderColor: 'divider' }}>
        <Tabs value={value} onChange={handleChange} aria-label="basic tabs example">
          <Tab label="Classic" {...a11yProps(0)} />
          <Tab label="Triples" {...a11yProps(1)} />
          <Tab label="Potholes" {...a11yProps(2)} />
        </Tabs>
      </Box>
      <TabPanel value={value} index={0}>
        {renderTab("classic")}
      </TabPanel>
      <TabPanel value={value} index={1}>
        {renderTab("triples")}
      </TabPanel>
      <TabPanel value={value} index={2}>
        {renderTab("potholes")}
      </TabPanel>
    </Box>
  );
}
