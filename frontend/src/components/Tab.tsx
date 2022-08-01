// @ts-nocheck
import * as React from 'react';
import Tabs from '@mui/material/Tabs';
import Tab from '@mui/material/Tab';
import Typography from '@mui/material/Typography';
import Box from '@mui/material/Box';
import Button from './ReusableButton'
import { useNavigate } from 'react-router-dom';

import { profileDataType } from '../pages/Profilepage';
import { leaderDataType } from '../pages/Leaderboard';

import Table from '@mui/material/Table';
import TableBody from '@mui/material/TableBody';
import TableCell, { tableCellClasses } from '@mui/material/TableCell';
import { styled } from '@mui/material/styles';
import TableContainer from '@mui/material/TableContainer';
import TableHead from '@mui/material/TableHead';
import TablePagination from '@mui/material/TablePagination';
import TableRow from '@mui/material/TableRow';

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

export default function StatTab({ data, value, setValue }: { 
  data: profileDataType
  value: number,
  setValue: React.Dispatch<React.SetStateAction<number>>
}) {

  const handleChange = (event: React.SyntheticEvent, newValue: number) => {
    setValue(newValue);
  };

  const renderTab = (type: string) => {
    // console.log(data.ranked["CLASSIC"])
    return(
      <Box display="flex">
        <Box width="400px" margin = "0 20px">
          <Typography variant="h5">Elo</Typography>
          <Typography variant="h5">Wins</Typography>
          <Typography variant="h5">Losses</Typography>
          <Typography variant="h5">Draws</Typography>
          
        </Box>
        <Box>
          <Typography variant="h5">{data.ranked[type].elo}</Typography>
          <Typography variant="h5">{data.ranked[type].wins}</Typography>
          <Typography variant="h5">{data.ranked[type].losses}</Typography>
          <Typography variant="h5">{data.ranked[type].draws}</Typography>
        </Box>
      </Box>
    )
  }

  return (
    <Box sx={{ width: '100%'}}>
      <Box sx={{ borderBottom: 1, borderColor: 'divider' }}>
        <Tabs value={value} 
        onChange={handleChange} 
        textColor=" " 
        indicatorColor="secondary"
        aria-label="secondary tabs example"
        >
          <Tab label="Classic" {...a11yProps(0)} />
          <Tab label="Triples" {...a11yProps(1)} />
          <Tab label="Potholes" {...a11yProps(2)} />
        </Tabs>
      </Box>
      <TabPanel value={value} index={0}>
        {renderTab("CLASSIC")}
      </TabPanel>
      <TabPanel value={value} index={1}>
        {renderTab("TRIPLES")}
      </TabPanel>
      <TabPanel value={value} index={2}>
        {renderTab("POTHOLES")}
      </TabPanel>
    </Box>
  );
}

const StyledTableCell = styled(TableCell)(({ theme }) => ({
  [`&.${tableCellClasses.head}`]: {
    backgroundColor: "var(--accent-darker)",
    color: theme.palette.common.white,
  },
  [`&.${tableCellClasses.body}`]: {
    fontSize: 14,
  },
}));

const StyledTableCell2 = styled(TableCell)(({ theme }) => ({
  [`&.${tableCellClasses.head}`]: {
    backgroundColor: "var(--accent-dark)",
    color: theme.palette.common.white,
  },
  [`&.${tableCellClasses.body}`]: {
    fontSize: 14,
  },
}));

const StyledTableRow = styled(TableRow)(({ theme }) => ({
  '&:nth-of-type(odd)': {
    backgroundColor: theme.palette.action.hover,
  },
  // hide last border
  '&:last-child td, &:last-child th': {
    border: 0,
  },
}));


export function LeaderTab({ data }: { data: leaderDataType}) {
  const [value, setValue] = React.useState(0);
  const navigate = useNavigate();

  const handleChange = (event: React.SyntheticEvent, newValue: number) => {
    setValue(newValue);
  };


  const renderTab = (type: string) => {
    // console.log(data["triples"])
    return(
        <Box width="1000px" margin = "0 20px" id="abc">
          {/* <Typography variant="h4">Elo</Typography>
          <Typography variant="h4">Wins</Typography>
          <Typography variant="h4">Losses</Typography>
          <Typography variant="h4">Draws</Typography>
          <Button background="purple" width={400}>Elo History Chart</Button> */}
          <TableContainer sx={{ maxHeight: 440, background: 'var(--accent-dark)' }}>
            <Table stickyHeader aria-label="sticky table">
              <TableHead>
                <TableRow>
                  <StyledTableCell align="center" style={{background: 'var(--accent-darker)'}}>Rank</StyledTableCell>
                  <StyledTableCell align="center" style={{background: 'var(--accent-darker)'}}>Name</StyledTableCell>
                  <StyledTableCell align="center" style={{background: 'var(--accent-darker)'}}>Elo</StyledTableCell>
                  <StyledTableCell align="center" style={{background: 'var(--accent-darker)'}}>Win</StyledTableCell>
                  <StyledTableCell align="center" style={{background: 'var(--accent-darker)'}}>Losses</StyledTableCell>
                </TableRow>
              </TableHead>
              <TableBody style={{verticalAlign:'top'}}>
                  {data[type]?.map((index) => (
                    <StyledTableRow>
                      <StyledTableCell2 align="center">{index.rank}</StyledTableCell2>
                      <StyledTableCell2
                        align="center"
                        style={{cursor: 'pointer'}}
                        onClick={() => {
                          navigate(`/profile/${index.uid}`)
                        }}
                      >{index.username}</StyledTableCell2>
                      <StyledTableCell2 align="center">{index.elo}</StyledTableCell2>
                      <StyledTableCell2 align="center">{index.wins}</StyledTableCell2>
                      <StyledTableCell2 align="center">{index.losses}</StyledTableCell2>
                    </StyledTableRow>
                  ))}
              </TableBody>
            </Table>
          </TableContainer>
        </Box>
    )
  }

  return (
    <Box sx={{ background: 'var(--accent-dark)'}} id="aslkjdlk">
      <Box sx={{ borderBottom: 1, borderColor: 'divider' }}>
        <Tabs value={value} 
        onChange={handleChange} 
        textColor=" " 
        indicatorColor="secondary"
        aria-label="secondary tabs example"
        >
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
